<?php declare(strict_types=1);

use PHPUnit\Framework\TestCase;

final class TimeoutTest extends TestCase
{
    public function testTimeoutNotTriggered(): void
    {
        $this->expectNotToPerformAssertions();

        enable_timeout(2);

        sleep(1);
    }
    
    public function testSimpleTimeout(): void
    {
        $this->expectExceptionMessage("Timeout of 1 seconds exceeded");

        enable_timeout(1);

        sleep(2);
    }

    public function testDeactivateTimeout(): void
    {
        $this->expectNotToPerformAssertions();

        enable_timeout(2);
        sleep(1);
        enable_timeout(0);
        sleep(2);
    }

    public function testTimeoutWithException(): void
    {
        $this->expectNotToPerformAssertions();

        enable_timeout(1);

        try {
            sleep(2);
        } catch (Exception $e) {
            // ignore
        }

        sleep(2); // should not throw an exception
    }
}