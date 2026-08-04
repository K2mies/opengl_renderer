import bpy

#print BSDF Values        
def print_material_colors():
    for obj in bpy.context.selected_objects:
        if obj.type != 'MESH':
            continue

        material = obj.active_material

        if material is None:
            print(f"{obj.name}: No active material")
            continue

        principled = next(
            (node for node in material.node_tree.nodes
             if node.type == 'BSDF_PRINCIPLED'),
            None
        )

        if principled is None:
            print(f"{obj.name}: No Principled BSDF")
            continue

        print(
            obj.name,
            material.name,
            tuple(principled.inputs["Base Color"].default_value)
        )

#assign active_materials
def assign_unique_active_materials():
    objects = sorted(
        (
            obj for obj in bpy.context.selected_objects
            if obj.type == 'MESH'
        ),
        key=lambda obj: obj.name
    )

    if not objects:
        print("No mesh objects selected")
        return

    for obj in objects:

    if len(obj.material_slots) == 0:
        print(f"{obj.name}: no material slots")
        continue

    obj.active_material_index = 0

    material = obj.active_material

    print(
        f"{obj.name}: slot 0 -> "
        f"{material.name if material else 'None'}"
    )

#Apply the active material colors to the POINT property.
def apply_active_material_colors():
    for obj in bpy.context.selected_objects:
        if obj.type != 'MESH':
            continue

        material = obj.active_material

        if material is None:
            print(f"{obj.name}: no active material")
            continue

        if not material.use_nodes:
            print(f"{obj.name}: material does not use nodes")
            continue

        principled = next(
            (
                node for node in material.node_tree.nodes
                if node.type == 'BSDF_PRINCIPLED'
            ),
            None
        )

        if principled is None:
            print(f"{obj.name}: no Principled BSDF found")
            continue

        material_color = tuple(
            principled.inputs["Base Color"].default_value
        )

        color_attributes = obj.data.color_attributes
        point_colors = color_attributes.get("PointColor")

        if point_colors is None:
            point_colors = color_attributes.new(
                name="PointColor",
                type='FLOAT_COLOR',
                domain='POINT'
            )
        elif point_colors.domain != 'POINT':
            print(
                f"{obj.name}: PointColor exists but "
                "is not on the POINT domain"
            )
            continue

        for color_element in point_colors.data:
            color_element.color = material_color

        obj.data.update()

        print(
            f"{obj.name}: {material.name} -> "
            f"{material_color}"
        )

#Verify the points have been assigned correctly
def verify_point_colors():
    for obj in bpy.context.selected_objects:
        if obj.type != 'MESH':
            continue

        material = obj.active_material
        if material is None:
            print(f"{obj.name}: No active material")
            continue

        principled = next(
            (node for node in material.node_tree.nodes
             if node.type == 'BSDF_PRINCIPLED'),
            None
        )

        if principled is None:
            print(f"{obj.name}: No Principled BSDF")
            continue

        expected = tuple(principled.inputs["Base Color"].default_value)

        point_colors = obj.data.color_attributes.get("PointColor")
        if point_colors is None:
            print(f"{obj.name}: No PointColor attribute")
            continue

        if len(point_colors.data) == 0:
            print(f"{obj.name}: No vertices")
            continue

        actual = tuple(point_colors.data[0].color)

        print(f"{obj.name}")
        print(f"  Material : {expected}")
        print(f"  Point    : {actual}")
        print(f"  Match    : {expected == actual}")
        print()

#Check that point data is preserved after joining the object
def print_point_colors():
    obj = bpy.context.object

    point_colors = obj.data.color_attributes.get("PointColor")

    if point_colors is None:
        print("No PointColor attribute found.")
        return

    print(f"{obj.name}: {len(obj.data.vertices)} vertices")

    for i, vertex in enumerate(obj.data.vertices):
        print(
            i,
            tuple(vertex.co),
            tuple(point_colors.data[i].color)
        )

assign_unique_active_materials()
apply_active_material_colors()
#print_material_colors()
verify_point_colors()

def remove_all_color_attributes():
    for obj in bpy.context.selected_objects:
        if obj.type != 'MESH':
            continue

        while obj.data.color_attributes:
            obj.data.color_attributes.remove(
                obj.data.color_attributes[0]
            )

        print(f"{obj.name}: removed all color attributes")


# Run the function
#remove_all_color_attributes()