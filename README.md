# Migrate Drupal 5 to Drupal 7

## Purpose of project

Drupal 7 module to automate the migration of data from Drupal 5 to Drupal 7. Data can be selectively migrated based on type.

## Migrations Available

+ nodes with revisions

+ users 

+ roles (only roles that are active)

+ user roles (only roles that are active)

+ permissions

+ menu

+ url alias

+ files (this is in rough shape, use with caution)

## Usage

+ Configure legacy database settings.

+ View the module page:

```
?q=admin/content/gemini_migrate_d5/url_alias
```

## Screenshot

![Alt text](docs/screenshot-node.png?raw=true "Screenshot of node tab")

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