#!/usr/bin/perl
use strict;
use warnings;

print "Content-Type: text/html\n\n";
print "<html><body>";
print "<h1>CGI Perl GET</h1>";
print "<p>QUERY_STRING: " . ($ENV{QUERY_STRING} // "(empty)") . "</p>";
print "<p>REQUEST_METHOD: " . ($ENV{REQUEST_METHOD} // "(empty)") . "</p>";
print "</body></html>";
