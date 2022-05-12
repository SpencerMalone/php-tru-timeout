<?php

enable_timeout(2);

echo "sleeping for 5s\n";
sleep(5);
echo "should not run\n";