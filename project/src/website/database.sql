-- CSE 474
-- Final Project

-- This is the SQL setup file for coordinate storage.

-- Set up database theDatabase
DROP DATABASE IF EXISTS theDatabase;
CREATE DATABASE theDatabase;
USE theDatabase;

-- Create Table
CREATE TABLE coords (
    x INT,
    y INT
);

-- Insert initial Value
INSERT INTO coords (x, y) VALUES (0, 0);