extends Node

func _process(_delta) -> void:
	if Input.is_action_just_pressed("ui_accept"):
		var mi = MonitorInfo.new()
		var monitors = mi.get_info()
		for monitor in monitors:
			var mon = monitor as Monitor
			print("Monitor Name: %s (%s)%s" % [mon.friendly_name, mon.device_name, " - PRIMARY" if mon.is_primary else ""])
			var resolutions = mon.resolutions;
			for resolution in resolutions:
				var resolution_size = resolution.size
				print("\t- %dx%d (%dhz)" % [resolution_size.x, resolution_size.y, resolution.refresh_rate]);
