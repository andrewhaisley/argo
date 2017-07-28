# Argonaut
Another C++ JSON Library. If you're familiar with the Python json
module this library should be easy to pick up. E.g. loading and
saving a JSON message is as simple as:

```
auto j = parser::load("file.json");
unparser::save(*j, "newfile.json");
```

# Documentation
There is some fairly [full documentation](https://andrewhaisley.github.io/).
