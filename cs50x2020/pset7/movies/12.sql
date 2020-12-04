SELECT movies.title
FROM
(
SELECT stars.movie_id
FROM people
JOIN stars
ON stars.person_id = people.id
WHERE people.name = "Johnny Depp"

INTERSECT

SELECT stars.movie_id
FROM people
JOIN stars
ON stars.person_id = people.id
WHERE people.name = "Helena Bonham Carter"
)
JOIN movies
ON movies.id = movie_id