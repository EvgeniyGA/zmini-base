# 2025-03-26T11:13:50.737289
import vitis

client = vitis.create_client()
client.set_workspace(path="ws")
#client.update_workspace(path="ws")
#print("asdfasdf")

platform = client.create_platform_component(name = "platform",hw_design = "$COMPONENT_LOCATION/../../../../xsa/bd_wrapper.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0")
#platform = client.create_platform_component(name = "platform",hw_design = "$COMPONENT_LOCATION/../../../../xsa/design_1_wrapper.xsa",os = "standalone",cpu = "microblaze_0",domain_name = "standalone_microblaze_0")

platform = client.get_component(name="platform")
status = platform.build()

comp = client.create_app_component(name="hello_world",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_ps7_cortexa9_0")
#comp = client.create_app_component(name="hello_world",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_microblaze_0")

comp = client.get_component(name="hello_world")
status = comp.import_files(from_loc="", files=["src"])

status = platform.build()

comp.build()

vitis.dispose()

