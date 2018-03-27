import dxl as Dxl
import math
ports = Dxl.get_available_ports()
print "connecting to port: %s" % (ports[0])

dxl = Dxl.Dxl(ports[0])

ids = dxl.scan(25)
print ids
dxl.enable_torque(ids)
beg = end = 100
for pos in range(-90, 91, 3):
    speed = -int(1000 * (math.sin(math.pi * (pos - 90) / 180)))
    dxl.set_moving_speed({1: speed})
    dxl.set_goal_position({1:pos})
for pos in range(90, -91, -3):
    speed = -int(1000 * (math.sin(math.pi * (pos - 90) / 180)))
    dxl.set_moving_speed({1: speed})
    dxl.set_goal_position({1:pos})
