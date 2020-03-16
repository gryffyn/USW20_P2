# USW20_P2

![Build](https://github.com/gryffyn/USW20_P2/workflows/Build/badge.svg)

USW Cyber Lab digital storage application.

## Building
---
### Cloning

Clone the repo, and then run `git submodule update --init --recursive`.

### Prerequisites

Requires MariaDB, and yaml-cpp (libyaml-cpp-dev on Ubuntu).

## Usage
---
### Setup

Create a Mariadb database - 
`mysql -u root -p`
```
MariaDB> CREATE DATABASE 'some_database_name';
MariaDB> CREATE USER 'some_user'@'localhost' IDENTIFIED BY 'some_pass';
MariaDB> GRANT ALL PRIVILEGES ON 'some_database_name'.* TO 'some_user'@'localhost';
MariaDB> FLUSH PRIVILEGES;
MariaDB> quit
```
where 
* `some_database_name` is a name for the database (`uswp20v2` is recommended)
* `some_user` is a username (`uswp20` is recommended)
* `some_pass` is a password to access the database

### Configuration

The file `config.yml` contains the settings for connecting to the Mariadb database. If you use the recommended settings for the database, you will not need to change anything except for the password.

---
## License

All code is licensed under the MIT License.
