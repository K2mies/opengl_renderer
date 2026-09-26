import bpy

obj = bpy.context.active_object
mesh = obj.data

attribute_names = (
    "PointColor",
    "ShadowColor",
)

# Preserve the existing values before removing the attributes.
saved_colors = {}

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

# Remove both FLOAT_COLOR attributes first.
for name in attribute_names:
    attribute = mesh.color_attributes.get(name)

    if attribute is not None:
        mesh.color_attributes.remove(attribute)

# Recreate them in the desired COLOR_0/COLOR_1 order.
for name in attribute_names:
    attribute = mesh.color_attributes.new(
        name=name,
        type="BYTE_COLOR",
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

# Make PointColor the active colour attribute.
mesh.color_attributes.active_color_index = 0

for attribute in mesh.color_attributes:
    print(
        attribute.name,
        attribute.data_type,
        attribute.domain,
        tuple(attribute.data[0].color)
    )