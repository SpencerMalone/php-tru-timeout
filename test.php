<?php

enable_timeout(2);

echo "sleeping for 5s\n";
try {
    sleep(5);
} catch (Exception $e) {
    echo "Caught exception: " . $e->getMessage() . "\n";
}
echo "runs after catch\n";