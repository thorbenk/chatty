from chatty import *
import time

log = Logger.getInstance()
log.setLogfile('/tmp/chatty.log')

log.log(Format.header1, Color.green, 'Hello world!')
log.warn('warning: starting example')

example_timer = log.startNewTimer('run example')

with IndentationContextManager() as l:
    l.log(Color.yellow, "indented log")

with TimedLineContextManager('long operation'):
    time.sleep(0.7)

N = 10
with ProgressLoopContextManager('long loop', N) as l:
    for i in range(10):
        time.sleep(0.4)
        l.advanceProgressLoop(1)

log.endTimer(example_timer)

time.sleep(2)
