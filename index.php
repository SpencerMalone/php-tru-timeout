<?php

enable_timeout(1);

echo "sleeping for 600s\n";
flush();
try {
    sleep(600);
} catch (Exception $e) {
    echo "Caught exception: " . $e->getMessage() . "\n";
}
echo "runs after catch\n";
flush();