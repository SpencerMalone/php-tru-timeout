<?php declare(strict_types=1);

use PHPUnit\Framework\TestCase;

final class TimeoutTest extends TestCase
{
    public function testTimeoutNotTriggered(): void
    {
        $this->expectNotToPerformAssertions();

        enableTimeout(2);

        sleep(1);
    }
    
    public function testSimpleTimeout(): void
    {
        $this->expectExceptionMessage("Timeout of 1 seconds exceeded");
        $this->expectException(TimeoutException::class);

        enableTimeout(1);

        sleep(2);
    }

    public function testDeactivateTimeout(): void
    {
        $this->expectNotToPerformAssertions();

        enableTimeout(2);
        sleep(1);
        enableTimeout(0);
        sleep(2);
    }

    public function testTimeoutWithException(): void
    {
        $this->expectNotToPerformAssertions();

        enableTimeout(1);

        try {
            sleep(2);
        } catch (Exception $e) {
            // ignore
        }

        sleep(2); // should not throw an exception
    }

    // When we fork, the timeout will not be maintained on the child.
    public function testFork(): void
    {
        enableTimeout(1);

        $pid = pcntl_fork();

        if ($pid == -1) {
            throw new Exception("fork failed");
        } elseif ($pid) {
            enableTimeout(4);
            $status = 0;
            pcntl_waitpid($pid, $status);
            $exitcode = pcntl_wexitstatus($status);
            $this->assertEquals(0, $exitcode);
        } else {
            // This should go through because timers are not maintained on the child of a fork.
            sleep(2);
            exit(0);
        }
    }

    public function testForkWithTimeouts(): void
    {
        enableTimeout(10);

        $pid = pcntl_fork();

        if ($pid == -1) {
            throw new Exception("fork failed");
        } elseif ($pid) {
            $status = 0;
            pcntl_waitpid($pid, $status);
            $exitcode = pcntl_wexitstatus($status);
            $this->assertEquals(124, $exitcode);
        } else {
            enableTimeout(1);
            try {
                sleep(3);
            } catch (TimeoutException $e) {
                exit(124);
            }
            exit(0);
        }
    }

    private function recognizableStackTrace(): void
    {
        sleep(10);
    }

    public function testStackTrace(): void
    {
        enableTimeout(1);

        try {
            $this->recognizableStackTrace();
        } catch (Exception $e) {
            $this->assertStringContainsString('recognizableStackTrace', $e->getTraceAsString());
        }
    }

    public function testGetTimer(): void
    {
        enableTimeout(3);

        // We're doing an extra long timeout here to handle the microsecond + second logic of getitimer
        sleep(1);

        $this->assertGreaterThan(1.99, getTimeUntilTimeout());
        $this->assertLessThan(2.01, getTimeUntilTimeout());
    }
}