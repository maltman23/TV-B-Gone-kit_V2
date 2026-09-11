# TV-B-Gone V2 MicroPython entry point. License: CC BY-SA 4.0.
from tvbgone import TVBGone

tv = TVBGone()
try:
    tv.run()
finally:
    tv.deinit()
