SELECT DISTINCT people.name

FROM
(
	SELECT directors.person_id AS dir_id, ratings.rating
	FROM ratings
	JOIN directors
	ON ratings.movie_id = directors.movie_id
)
JOIN people
ON people.id = dir_id
WHERE rating >= 9.0 AND rating != "\N"