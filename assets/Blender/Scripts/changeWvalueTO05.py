mesh = bpy.context.active_object.data
attribute = mesh.color_attributes["PointColor"]

for value in attribute.data:
    color = value.color

    value.color = (
        color[0],
        color[1],
        color[2],
        0.5
    )
#Converts the w vallue to 0.5 to trick blender into exporting a full RGBA attribute.