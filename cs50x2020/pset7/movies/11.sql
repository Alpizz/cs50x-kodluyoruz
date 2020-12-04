SELECT mv_t
FROM
(
	SELECT movies.id as mv_id, movies.title as mv_t
	FROM movies
	JOIN stars
	ON stars.movie_id = movies.id
	WHERE stars.person_id = (SELECT people.id FROM people WHERE people.name LIKE "Chadwick Boseman")
)
JOIN ratings
ON ratings.movie_id = mv_id
ORDER BY rating DESC
LIMIT 5