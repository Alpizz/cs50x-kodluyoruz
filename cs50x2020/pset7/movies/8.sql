SELECT people.name
FROM movies, people
JOIN stars
ON stars.movie_id = movies.id AND people.id = stars.person_id
WHERE title = "Toy Story"