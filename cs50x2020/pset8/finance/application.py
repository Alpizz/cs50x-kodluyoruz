import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Initialize database
db.execute("""CREATE TABLE IF NOT EXISTS 'users'
              ('id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 'username' TEXT NOT NULL,
              'hash' TEXT NOT NULL, 'cash' NUMERIC NOT NULL DEFAULT 10000.00)
              """)
db.execute("CREATE TABLE IF NOT EXISTS sqlite_sequence(name,seq)")
db.execute("CREATE UNIQUE INDEX IF NOT EXISTS'username' ON 'users' ('username')")
db.execute("""CREATE TABLE IF NOT EXISTS 'stock_data'
              ('share_id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 'user_id' INTEGER NOT NULL,
              'stock_name' TEXT NOT NULL, 'share_count' NUMERIC NOT NULL)
              """)
db.execute("""CREATE TABLE IF NOT EXISTS 'history'
              ('unique_trans_id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
              'user_id' INTEGER NOT NULL, 'stock_name' TEXT NOT NULL, 'change' INTEGER NOT NULL,
              'price' FLOAT NOT NULL, Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)
              """)

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get stocks info for user
    stock_rows = db.execute("SELECT u.id, d.stock_name, d.share_count FROM stock_data d "
                            "INNER JOIN users u ON u.id = d.user_id WHERE u.id = :id AND d.share_count > 0",
                            id=session["user_id"])

    # Get cash info
    balance = db.execute("SELECT cash FROM users WHERE id = :id",
                          id=session["user_id"])[0]['cash']

    # Initialize empty dict and list to populate with stock info
    stock_info = {}
    stock_list = []
    # Initial grand total is equal to balance
    grand_total = balance

    # Go through every stock row
    for row in stock_rows:
        # Get neccesary info from SQL query and from the internet with lookup
        symbol = row["stock_name"]
        share_count = row["share_count"]
        firm_dict = lookup(symbol)
        longname = firm_dict['name']
        price = firm_dict['price']

        # Calculate total price of the individual stock
        small_total = price * share_count

        # Save these info to the stock_info dictionary
        stock_info['symbol'] = symbol
        stock_info['name'] = longname
        stock_info['shares'] = share_count
        stock_info['price'] = usd(price)
        stock_info['total'] = usd(small_total)

        # Add temporary dictionary to the list
        stock_list.append(stock_info)

        # Increase grand total
        grand_total += small_total

        # Empty individual stock dictionary
        stock_info = {}

    # Render index with given parameters
    return render_template("index.html", stock_list=stock_list, grand_total=usd(grand_total), balance=usd(balance))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # Get symbol from form
        symbol = request.form.get("symbol").upper()
        if not symbol:
            return apology("missing symbol", 400)

        # Get share count from form and convert into float if present
        share_buy = request.form.get("shares")
        if not share_buy:
            return apology("missing shares", 400)
        share_buy = int(share_buy)

        # Lookup for that symbol
        firm_dict = lookup(symbol)

        # Ensure symbol is valid
        if not firm_dict:
            return apology("invalid symbol", 400)

        # Calculate total amount of price
        total = firm_dict['price'] * share_buy

        # Get user balance from database
        user_balance = db.execute("SELECT cash FROM users WHERE id = :id",
                                   id=session["user_id"])

        # Check for affordability
        if user_balance[0]['cash'] < total:
            return apology("can\'t afford", 400)

        # Query for checking existing stocks
        existing_stock = db.execute("SELECT share_id FROM stock_data WHERE user_id = :id AND stock_name = :symbol",
                                    id=session["user_id"], symbol=symbol)

        # Insert if user is buying a stock first time
        if len(existing_stock) == 0:
            db.execute("INSERT INTO stock_data (user_id, stock_name, share_count) VALUES (:id, :symbol, :share_buy)",
                        id=session["user_id"], symbol=symbol, share_buy=share_buy)

        # Update if user has that stock
        else:
            db.execute("UPDATE stock_data SET share_count = share_count + :share_buy WHERE user_id = :id AND stock_name = :symbol",
                        share_buy=share_buy, id=session["user_id"], symbol=symbol)


        # Decrease amount from user's balance
        db.execute("UPDATE users SET cash = cash - :total WHERE id = :id",
                    total=total, id=session["user_id"])

        # Record to the history
        db.execute("INSERT INTO history (user_id, stock_name, change, price) VALUES (:id, :symbol, :change, :price)",
                    id=session["user_id"], symbol=symbol, change=+share_buy, price=firm_dict['price'])

        # Redirect to home
        flash(f"Bought {share_buy} {firm_dict['name']} {firm_dict['symbol']} stocks from {usd(firm_dict['price'])}!")
        return redirect("/")

    # Render buy page
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query for history information
    tra_list = db.execute("SELECT stock_name, change, price, Timestamp FROM history WHERE user_id = :id",
                           id=session["user_id"])

    # Do not try to pass list if no history found
    if tra_list == None:
        return render_template("history.html")

    # Convert every price value to USD format
    for tr in tra_list:
        tr['price'] = usd(tr['price'])

    # Render history page
    return render_template("history.html", tra_list=tra_list)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash(f"Welcome back, {request.form.get('username')}")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""
    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # Get symbol from form
        symbol = request.form.get("symbol").upper()

        if not symbol:
            return apology("missing symbol", 400)

        # Lookup for that symbol
        firm_dict = lookup(symbol)

        # Ensure symbol is valid
        if not firm_dict:
            return apology("invalid symbol", 400)

        # Store name, symbol, price values of firm_dict in a formatted string
        quoted_text = f"A share of { firm_dict['name'] } ({ firm_dict['symbol'] }) costs { usd(firm_dict['price']) }."

        # Render template with given string
        return render_template("quoted.html", quoted_text=quoted_text)

    # Render page if method is GET
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted twice
        elif not request.form.get("password_first") or not request.form.get("password_second"):
            return apology("must provide password", 403)

        # Ensure passwords do match
        elif not request.form.get("password_first") == request.form.get("password_second"):
            return apology("passwords must match", 403)

        # Query database for checking username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username is unique
        if len(rows) != 0:
            return apology("username already taken", 403)

        # Hash user password
        hashpwd = generate_password_hash(request.form.get("password_second"))

        # Insert new user into database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                    username = request.form.get("username"),
                    hash = hashpwd)

        flash('Registered successfully! Sign in to continue.')
        # Redirect to mainpage
        return redirect("/")

    # Render register page if the method is GET
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Get symbols currently user has
    symbols = db.execute("SELECT stock_name FROM stock_data WHERE user_id = :id AND share_count > 0",
                          id=session["user_id"])

    if request.method == "POST":
        # Get stock symbol from form
        symbol = request.form.get("symbol")

        # Check for missing symbol
        if not symbol:
            return apology("missing symbol", 400)

        # Get share count to sell from form
        share_sell = request.form.get("shares")

        # Check for missing count
        if not share_sell:
            return apology("missing shares", 400)

        # Convert count to integer
        share_sell = int(share_sell)

        # Query for selected stock
        share_count = db.execute("SELECT share_count FROM stock_data WHERE user_id = :id AND stock_name = :symbol",
                                  id=session["user_id"], symbol=symbol)[0]['share_count']

        # Check for too many shares
        if share_sell > share_count:
            return apology("too many shares", 400)

        # Decrease shares from portfoilo
        db.execute("UPDATE stock_data SET share_count = share_count - :share_sell WHERE user_id = :id AND stock_name = :symbol",
                        share_sell=share_sell, id=session["user_id"], symbol=symbol)

        # Get price information from the internet for the symbol
        firm_dict = lookup(symbol)

        # Calculate income
        gain = firm_dict['price'] * share_sell

        # Increase amount from user's balance
        db.execute("UPDATE users SET cash = cash + :gain WHERE id = :id",
                    gain=gain, id=session["user_id"])

        # Record to the history
        db.execute("INSERT INTO history (user_id, stock_name, change, price) VALUES (:id, :symbol, :change, :price)",
                    id=session["user_id"], symbol=symbol, change=-share_sell, price=firm_dict['price'])

        # Redirect to index
        flash(f"Sold {share_sell} {firm_dict['name']} (\'{firm_dict['symbol']}\') stocks from {usd(firm_dict['price'])}!")
        return redirect("/")

    else:
        # Render sell page with symbols parameter
        return render_template("sell.html", symbols=symbols)

@app.route("/clear-history", methods=["POST"])
def clear():
    """Clear all history information"""

    # Delete all rows of history of user
    db.execute("DELETE FROM history WHERE user_id = :id",
                id=session["user_id"])

    # Redirect to history
    return redirect("/history")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
