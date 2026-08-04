import bpy


def print_material_colors():
    for obj in bpy.context.selected_objects:
        if obj.type != 'MESH':
            continue

        material = obj.active_material

        if material is None:
            print(f"{obj.name}: no active material")
            continue

        print(
            f"{obj.name}: "
            f"{material.name} -> "
            f"{tuple(material.diffuse_color)}"
        )


def assign_active_material_slot_zero():
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


def apply_active_material_colors():
    for obj in bpy.context.selected_objects:
        if obj.type != 'MESH':
            continue

        material = obj.active_material

        if material is None:
            print(f"{obj.name}: no active material")
            continue

        material_color = tuple(material.diffuse_color)

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


def verify_point_colors():
    for obj in bpy.context.selected_objects:
        if obj.type != 'MESH':
            continue

        material = obj.active_material

        if material is None:
            print(f"{obj.name}: no active material")
            continue

        expected = tuple(material.diffuse_color)

        point_colors = obj.data.color_attributes.get("PointColor")

        if point_colors is None:
            print(f"{obj.name}: no PointColor attribute")
            continue

        if len(point_colors.data) == 0:
            print(f"{obj.name}: no vertices")
            continue

        actual = tuple(point_colors.data[0].color)

        print(f"{obj.name}")
        print(f"  Material : {expected}")
        print(f"  Point    : {actual}")
        print(f"  Match    : {expected == actual}")
        print()


def print_point_colors():
    obj = bpy.context.object

    if obj is None:
        print("No active object")
        return

    if obj.type != 'MESH':
        print(f"{obj.name}: active object is not a mesh")
        return

    point_colors = obj.data.color_attributes.get("PointColor")

    if point_colors is None:
        print("No PointColor attribute found")
        return

    print(f"{obj.name}: {len(obj.data.vertices)} vertices")

    for i, vertex in enumerate(obj.data.vertices):
        print(
            i,
            tuple(vertex.co),
            tuple(point_colors.data[i].color)
        )


def prepare_selected_objects():
    assign_active_material_slot_zero()
    apply_active_material_colors()
    verify_point_colors()


prepare_selected_objects()
#print_point_colors()