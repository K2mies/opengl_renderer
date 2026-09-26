import bpy

obj = bpy.context.active_object
mesh = obj.data

attribute_names = (
    "PointColor",
    "ShadowColor",
)

saved_colors = {}

# Preserve the RGB values.
for name in attribute_names:
    attribute = mesh.color_attributes.get(name)

    if attribute is None:
        raise RuntimeError(
            f"Missing color attribute: {name}"
        )

    saved_colors[name] = [
        tuple(element.color)
        for element in attribute.data
    ]

# Remove the current BYTE_COLOR attributes.
for name in attribute_names:
    attribute = mesh.color_attributes.get(name)

    if attribute is not None:
        mesh.color_attributes.remove(attribute)

# Recreate both as FLOAT_COLOR in COLOR_0/COLOR_1 order.
for name in attribute_names:
    attribute = mesh.color_attributes.new(
        name=name,
        type="FLOAT_COLOR",
        domain="POINT"
    )

    for element, old_color in zip(
        attribute.data,
        saved_colors[name]
    ):
        element.color = (
            old_color[0],
            old_color[1],
            old_color[2],
            0.5
        )

# PointColor should become COLOR_0.
mesh.color_attributes.active_color_index = 0
mesh.color_attributes.render_color_index = 0

for index, attribute in enumerate(mesh.color_attributes):
    print(
        index,
        attribute.name,
        attribute.data_type,
        attribute.domain,
        tuple(attribute.data[0].color)
    )