<?php

enable_timeout(2);

try {
    sleep(600);
} catch (Exception $e) {
    echo "Caught exception: " . $e->getMessage() . "\n";
}
echo "runs after catch\n";