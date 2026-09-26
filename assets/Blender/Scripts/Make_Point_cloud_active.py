import bpy

mesh = bpy.context.active_object.data

for index, attribute in enumerate(mesh.color_attributes):
    if attribute.name == "PointColor":
        mesh.color_attributes.active_color_index = index
        mesh.color_attributes.render_color_index = index
        break

for index, attribute in enumerate(mesh.color_attributes):
    print(
        index,
        attribute.name,
        attribute.data_type,
        attribute.domain,
        "<-- active"
        if index == mesh.color_attributes.active_color_index
        else ""
    )