import dxl as Dxl

ports = Dxl.get_available_ports()
print "connecting to port: %s" % (ports[0])

dxl = Dxl.Dxl(ports[0])

ids = dxl.scan(25)

dxl.set_goal_position()
