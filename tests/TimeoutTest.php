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
        $this->expectException(TimeoutException::class);

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

    // When we fork, the timeout will not be maintained on the child.
    public function testFork(): void
    {
        enable_timeout(1);

        $pid = pcntl_fork();

        if ($pid == -1) {
            throw new Exception("fork failed");
        } elseif ($pid) {
            enable_timeout(4);
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
        enable_timeout(1);

        $pid = pcntl_fork();

        if ($pid == -1) {
            throw new Exception("fork failed");
        } elseif ($pid) {
            enable_timeout(4);
            $status = 0;
            pcntl_waitpid($pid, $status);
            $exitcode = pcntl_wexitstatus($status);
            $this->assertEquals(2, $exitcode);
        } else {
            enable_timeout(1);
            sleep(2);
            exit(0);
        }
    }

    private function recognizableStackTrace(): void
    {
        sleep(10);
    }

    public function testStackTrace(): void
    {
        enable_timeout(1);

        try {
            $this->recognizableStackTrace();
        } catch (Exception $e) {
            $this->assertStringContainsString('recognizableStackTrace', $e->getTraceAsString());
        }
    }
}