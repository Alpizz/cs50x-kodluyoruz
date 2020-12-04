SELECT movies.title, ratings.rating
FROM movies JOIN ratings on movies.id = ratings.movie_id
WHERE year = 2010 AND rating != "\N"
ORDER BY rating DESC, title ASC;