from _chatty import Logger, Color, Format
import time, re

l = Logger.getInstance()
l.setLogfile("/tmp/unittest__pylogger.txt")

#
# Color
#
l.log(Color.red, "red text")
l.log(Color.green, "green text")
l.log(Color.yellow, "yellow text")
l.log(Color.blue, "blue text")
l.log(Color.magenta, "magenta text")
l.log(Color.cyan, "cyan text")

#
# Header
#
l.log(Format.header1, Color.blue, "blue heading 1")
l.log(Format.header2, Color.blue, "blue heading 2")
l.log(Format.header3, Color.blue, "blue heading 3")
l.indent()
l.log("indented text")
l.unindent()
l.log(Color.red, "red text")

timer = l.startNewTimer("test_timer")
l.endTimer(timer)

l.logN("a")
l.logN("b")
l.logN("c")
l.logFlushline()

#
# Progress Loop
#
with l.progressLoop("my loop", 3):
    for i in range(3):
        l.advanceProgressLoop(1)

#
# Timer
#
with l.logTimed("testing timed line"):
    pass

l.log("indent")
with l.indentation():
    l.log("indented?")

l.log("now make a loop")
l.indent()
l.beginProgressLoop(3)
for i in range(3):
    l.advanceProgressLoop(1)
l.endProgressLoop()
l.unindent()

s = l.formatHTML()
f = open("/tmp/unittest__pylogger.html", 'w')
f.write(s)
f.close()

expectedOutput="""red text
green text
yellow text
blue text
magenta text
cyan text
*******************************************************************************
blue heading 1
*******************************************************************************
blue heading 2
===============================================================================
blue heading 3
-------------------------------------------------------------------------------
  indented text
red text
starting timer 'test_timer'
timer 'test_timer' took 1.97887e-05s
abc
my loop: loop over 3 took 3.93391e-05s
testing timed line ... 1.66893e-06s
indent
  indented?
now make a loop
loop over 3 took 2.40803e-05s
"""
expectedOutput = re.sub("([-+]?\\d+\\.\\d+([eE][-+]?\\d+)?)", "NUMBER", expectedOutput)

f = open("/tmp/unittest__pylogger.txt", 'r')
s = "".join(f.readlines())
f.close()
s = re.sub("([-+]?\\d+\\.\\d+([eE][-+]?\\d+)?)", "NUMBER", s)

f = open("/tmp/a.txt", 'w')
f.write(expectedOutput)
f.close()
f = open("/tmp/b.txt", 'w')
f.write(s)
f.close()

if s!= expectedOutput:
    import difflib
    diff = difflib.ndiff(expectedOutput, s)
    try:
        while 1:
            print diff.next(),
    except:
        pass
    raise RuntimeError("Test failed")