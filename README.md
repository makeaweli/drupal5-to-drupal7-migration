# Migrate Drupal 5 to Drupal 7

## Purpose of project

Drupal 7 module to automate the migration of data from Drupal 5 to Drupal 7. Data can be selectively migrated based on type.

## Migrations Available

+ nodes with revisions

+ users 

+ roles

+ user roles

+ menu

+ url alias

+ files (this is in rough shape, use with caution)

## Usage

+ Configure legacy database settings.

+ View the module page:

```
?q=admin/content/gemini_migrate_d5/url_alias
```

## Configuration

Add a new database connection for your Drupal 5 database to your settings.php named 'legacy':

```php

$databases = array (
  'default' =>
  array (
    'default' =>
    array (
      'database' => 'drupal-7',
      'username' => 'deploy',
      'password' => '',
      'host' => 'localhost',
      'port' => '',
      'driver' => 'mysql',
      'prefix' => '',
    ),

  ),

  'legacy' =>
  array (
    'default' =>
    array (
      'database' => 'drupal-5',
      'username' => 'deploy',
      'password' => '',
      'host' => 'localhost',
      'port' => '',
      'driver' => 'mysql',
      'prefix' => '',
    )
  )
);  

```

## System Requirements

This module was developed on a vagrant box with the following configuration:

+ Linux Distribution: CentOS 5.11

+ System RAM: 512MB

+ PHP 5.2

+ PHP RAM: 256MB

## Current project maintainer

Jason Kalawe

## Dependencies

None

## Deployment URL

N/A

## Deployment filesystem location

N/A

## Example of repeatable output for testing

N/A

## Using Vagrant for development

# Configure SequelPro

```
mysql -u root -e "GRANT ALL PRIVILEGES ON *.* TO 'root'@'127.0.0.1' IDENTIFIED BY '' WITH GRANT OPTION; FLUSH PRIVILEGES;"
```

## SequelPro settings

+ MySQL Host: 127.0.0.1

+ Username: root

+ SSH Host: localhost

+ SSH User: vagrant

+ SSH Password: vagrant

+ SSh Port: 2222

