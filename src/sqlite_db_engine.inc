<?php
/*!
 * sqlite_db_engine
 *
 * Stefano Peloso - https://github.com/stefanoio/sqlite_db_engine
 *
 * Free to use under terms of WTFPL version 2 license
 */

trait concat {

	private static $that = null;

	private static function that() {
		if(empty(self::$that)) {
			$class = get_called_class();
			self::$that = new $class;
		}
		return self::$that;
	}

}

abstract class sqlite_db_engine {

	use concat;

	protected $database, $handler;

	function __construct() {
		$this->database = new SQLite3(get_called_class()::path);
	}

	static function query() {
		$that = self::that();
		$query = "";
		$escape = true;
		foreach(func_get_args() as $arg) {
			$query .= (($escape = !$escape) ? SQLite3::escapeString($arg) : $arg) . "'";
		}
		$that->handler = $that->database->query($escape ? $query : substr($query, 0, -1));
		return $that;
	}

	private static function doquery($args) {
		if(count($args)) {
			call_user_func_array([get_called_class(), "query"], $args);
		}
	}

	private static function fetchArray($num = false) {
		return self::that()->handler->fetchArray($num ? SQLITE3_NUM : SQLITE3_ASSOC);
	}

	static function value() {
		self::doquery(func_get_args());
		return self::fetchArray(true)[0];
	}

	static function row() {
		self::doquery(func_get_args());
		return self::fetchArray();
	}

	static function rowN() {
		self::doquery(func_get_args());
		return self::fetchArray(true);
	}

	static function values() {
		self::doquery(func_get_args());
		$values = [];
		while($row = self::fetchArray(true)) {
			$values []= $row[0];
		}
		return $values;
	}

	static function rows() {
		self::doquery(func_get_args());
		$rows=[];
		while($rows []= self::fetchArray());
		return array_slice($rows, 0, -1);
	}

	static function rowsN(){
		self::doquery(func_get_args());
		$rows=[];
		while($rows []= self::fetchArray());
		return array_slice($rows, 0, -1);
	}

	static function id() {
		self::doquery(func_get_args());
		return self::that()->database->lastInsertRowID();
	}

}
