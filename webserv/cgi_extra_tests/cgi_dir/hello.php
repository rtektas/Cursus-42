<?php
echo "Content-Type: text/html\r\n\r\n";
echo "<html><body>";
echo "<h1>CGI PHP GET</h1>";
echo "<p>QUERY_STRING: " . htmlspecialchars($_SERVER['QUERY_STRING'] ?? '(empty)') . "</p>";
echo "<p>REQUEST_METHOD: " . htmlspecialchars($_SERVER['REQUEST_METHOD'] ?? '(empty)') . "</p>";
echo "</body></html>";

