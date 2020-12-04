SELECT title, year FROM movies
WHERE (lower(title) LIKE "harry potter%")
ORDER BY year ASC;