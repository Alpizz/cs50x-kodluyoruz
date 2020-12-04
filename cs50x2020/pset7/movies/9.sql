SELECT people.name

FROM
(
	SELECT DISTINCT stars.person_id AS ids
	FROM stars
	JOIN movies
	ON stars.movie_id = movies.id
	WHERE movies.year = 2004
)
JOIN people
ON ids = people.id
ORDER BY people.birth ASC