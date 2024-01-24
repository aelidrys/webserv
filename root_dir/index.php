<?php
// Set the Content-Type header to specify that this is a CGI script
header("Content-Type: text/html");

// Read query parameters from the environment (for example, from a GET request)
$queryString = getenv('QUERY_STRING');
parse_str($queryString, $queryParams);

echo "env: $queryString";
// Extract values from the query parameters
$name = isset($queryParams['name']) ? $queryParams['name'] : 'Guest';

// Output HTML content
echo "<html>
<head>
    <title>CGI PHP Example</title>
</head>
<body>
    <h1>Hello, $name!</h1>
    <p>This is a simple PHP CGI script.</p>
</body>
</html>";
?>
