# sqlite_db_engine
## Convenient and secure PHP interface for sqlite databases.

sqlite_db_engine is a PHP class that provides easy to use and secure access to sqlite databases.
Requirements are:
- PHP 5.4+
- SQLite3 extension

To use it, just include it and create a new static instance:
```
require "sqlite_db_engine.inc";
class my_database extends sqlite_db_engine {
	const path="path/sqlite_file.extension";
}
```

sqlite_db_engine is designed with both static methods and internal instance concatenation. That means that you can both use
```
my_database::query("SELECT name FROM users WHERE username=", $username)->value();
```
and
```
my_database::value("SELECT name FROM users WHERE username=", $username);
```
syntax.
If you want you can even concatenate more queries:
```
my_database::query("UPDATE users SET last_login=NOW() WHERE username=", $username)->query("SELECT last_login FROM users WHERE username=", $username)->value();
```
or
```
my_database::query("UPDATE users SET last_login=NOW() WHERE username=", $username)->value("SELECT last_login FROM users WHERE username=", $username);
```

As you might have noticed, in those examples we used a peculiar syntax to form our query. sqlite_db_engine has an handy system that can take an array of strings and concatenate them adding the proper escape and quotes. For example:
```
query("SELECT * WHERE surname=", "O'connor", " AND (age>", 24, " OR age<", 35, ")")
```
will be compiled into:
```
query("SELECT * WHERE name='O\'connor' AND (age>'24' OR age<'35')")
```
Please note that in SQL languages there is no strict typing, so treating a number as a string has no downsides and this approach guarantee worry free security from both code bugs and malicious injections.

The main sqlite_db_engine’s method is `query`, which will return an instance of the class with the performed query. Any other methods will accept the same query format parameters (optionals, if no query is given the last query data is used) and return a specific value:
```
value		// returns a string containing the first value of the first row
values		// returns an array of strings representing the first values of every row
row			// returns an associative array representing the first row
rowN		// returns an array representing the first row
rows		// returns an array containing every row represented as an associative array
rowsN		// returns an array containing every row represented as an array
id			// returns the last insertion id
```
