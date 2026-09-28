<?php
$body = file_get_contents("php://input");
echo "Content-Type: text/html\r\n\r\n";
echo "<html><body>";
echo "<h1>CGI PHP POST</h1>";
echo "<p>REQUEST_METHOD: " . htmlspecialchars($_SERVER['REQUEST_METHOD'] ?? '(empty)') . "</p>";
echo "<p>BODY: " . htmlspecialchars($body) . "</p>";
echo "</body></html>";

