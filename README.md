# Migrate Drupal 5 to Drupal 7

## Purpose of project

Module to automate the migration of data from Drupal 5 to Drupal 7.

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

### Configure /etc/hosts

```
127.0.0.1 drupal-5.local
```


# Migration

## Modules

+ ldap

+ masquerade

+ hierarchial select

+ imagecache

+ menublock

+ pathauto

+ nodetype

+ scheduler

+ view unpublished content

+ CKFinder

+ content access

## Migrations

+ nodes

  + Book page (book)

  + Development (development)

  + Engineering (engineering)

  + Homepage (hp)

  + Obs Planning (obs_plan)

  + Safety & Health (safety)

  + Science Operations (sciops)

  + Story (story)

  + The Observer Newsletter (observer)

+ revisions

+ users 

+ roles

+ user roles

+ menu

  + Navigation

  + Safety

  + Science Operations

+ pathauto

+ files

+ views


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

