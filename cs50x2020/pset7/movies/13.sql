SELECT people.name
FROM
(
	SELECT DISTINCT stars.person_id as all_id
	FROM
	(
		SELECT stars.movie_id as kb_mv
		FROM stars
		WHERE stars.person_id =
		(
			SELECT people.id
			FROM people
			WHERE people.name = "Kevin Bacon" AND people.birth = 1958
		)
	)
	JOIN stars
	ON stars.movie_id = kb_mv
)
JOIN people
ON people.id = all_id
WHERE people.name != "Kevin Bacon"