# Argonaut
A flexible and easy to use JSON library that makes the mapping of 
a structures from a dynamically typed language onto C++ objects 
as painless as possible. Support for JSON pointer (aka JSON Path)
is included.

If you're familiar with the Python json module this library should 
be very easy to pick up. E.g. loading and saving a JSON message is as 
simple as:

```
auto j = parser::load("file.json");
unparser::save(*j, "newfile.json");
```

# Documentation
There is full documentation [here](https://andrewhaisley.github.io/argonaut/).
