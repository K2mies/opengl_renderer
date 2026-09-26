import bpy      
import bpy


def create_shadow_color_gradient():
    for obj in bpy.context.selected_objects:
        if obj.type != 'MESH':
            continue

        mesh = obj.data

        # Colors at the bottom and top of the gradient.
        bottom_color = (0.1, 0.0, 0.2, 0.5)
        top_color    = (1.0, 0.2, 0.0, 0.5)

        color_attributes = mesh.color_attributes
        shadow_colors = color_attributes.get("ShadowColor")

        if shadow_colors is None:
            shadow_colors = color_attributes.new(
                name="ShadowColor",
                type='FLOAT_COLOR',
                domain='POINT'
            )

        elif shadow_colors.domain != 'POINT':
            print(
                f"{obj.name}: ShadowColor exists but "
                "is not on the POINT domain"
            )
            continue

        # Find the vertical range of the object.
        z_positions = [
            vertex.co.z
            for vertex in mesh.vertices
        ]

        if not z_positions:
            print(f"{obj.name}: mesh contains no vertices")
            continue

        minimum_z = min(z_positions)
        maximum_z = max(z_positions)

        z_range = maximum_z - minimum_z

        # Avoid division by zero for a completely flat object.
        if z_range == 0.0:
            z_range = 1.0

        for vertex, color_element in zip(
            mesh.vertices,
            shadow_colors.data
        ):
            # Convert the vertex's Z position to a value from 0 to 1.
            gradient_position = (
                vertex.co.z - minimum_z
            ) / z_range

            red = (
                bottom_color[0]
                + (
                    top_color[0] - bottom_color[0]
                )
                * gradient_position
            )

            green = (
                bottom_color[1]
                + (
                    top_color[1] - bottom_color[1]
                )
                * gradient_position
            )

            blue = (
                bottom_color[2]
                + (
                    top_color[2] - bottom_color[2]
                )
                * gradient_position
            )

            alpha = (
                bottom_color[3]
                + (
                    top_color[3] - bottom_color[3]
                )
                * gradient_position
            )

            color_element.color = (
                red,
                green,
                blue,
                alpha
            )

        mesh.update()

        print(
            f"{obj.name}: created ShadowColor gradient "
            f"from Z {minimum_z:.3f} to {maximum_z:.3f}"
        )

def create_second_attribute_red():
    for obj in bpy.context.selected_objects:
        if obj.type != 'MESH':
            continue
        
        #red color for testing purposes
        color = tuple((1.0, 0.0, 0.0, 0.5)) #the w is 0.5 so it is not culled by exporter/importer
        
        color_attributes = obj.data.color_attributes
        shadow_colors = color_attributes.get("ShadowColor")
        
        if shadow_colors is None:
            shadow_colors = color_attributes.new(
                name="ShadowColor",
                type='FLOAT_COLOR',
                domain='POINT'
            )
        elif shadow_colors.domain != 'POINT':
            print(f"{obj.name: ShadowColor exists but} ""is not on the POINT domain")
            continue
        
        for color_element in shadow_colors.data:
            color_element.color = color
        
        obj.data.update()
        
        point_colors    = obj.data.color_attributes.get("PointColor")
        
        actual_point    = tuple(point_colors.data[0].color)
        actual_shadow   = tuple(shadow_colors.data[0].color)
        
        print(f"{obj.name}")
        print(f"  Point    : {actual_point}")
        print(f"  Shadow   ; {actual_shadow}")
        print()


def print_color_attributes():
    for obj in bpy.context.selected_objects:
        if obj.type != 'MESH':
            continue
        
        if obj is None:
            print("No active object")
        
        point_colors = obj.data.color_attributes.get("PointColor")
        shadow_colors = obj.data.color_attributes.get("ShadowColor")
        
        if point_colors is None:
            print("No PointColor attribute found")
            return
        
        elif shadow_colors is None:
            print("No ShadowColor attribute found")
        
        for i, vertex in enumerate(obj.data.vertices):
            print(
                    i, 
                    tuple(vertex.co), 
                    tuple(point_colors.data[i].color), 
                    tuple(shadow_colors.data[i].color)
            )

#def create_and_print_attributes():
#    create_second_attribute_red()
#    print_color_attributes()

#create_and_print_attributes()

#print_color_attributes()
def create_and_print_gradient():
    create_shadow_color_gradient()
    print_color_attributes()

create_and_print_gradient()
        