import bpy

#we neeed to get the shadow and color values from the ramp node
#we need to create two seperate color attributes
#need to feed the shadow and color values into it

#fetch the primary color from the color ramp
def get_primary_color(obj):
    material   = obj.active_material
    node       = material.node_tree.nodes['Color Ramp']
    color_ramp = node.color_ramp
    elements   = color_ramp.elements
    
    color      = elements[1].color
    
    return (color[0], color[1], color[2])

    return None
#fetch the secondary color from the color ramp
def get_secondary_color(obj):
    material   = obj.active_material
    node       = material.node_tree.nodes['Color Ramp']
    color_ramp = node.color_ramp
    elements   = color_ramp.elements
    
    color      = elements[0].color
    
    return (color[0], color[1], color[2])

    return None

#set the primary custom attribute

    
def set_primary_attribute(obj):

    primary_color = get_primary_color(obj)

    color_attributes = obj.data.color_attributes
    color_attribute  = color_attributes.get("PrimaryColor")

    if color_attribute is None:
        color_attribute = color_attributes.new(
            name="PrimaryColor",
            type='FLOAT_COLOR',
            domain='POINT'
        )

    elif color_attribute.domain != 'POINT':
        print(f"{obj.name}: PrimaryColor exists but is not on the POINT domain")
        return

    for color_element in color_attribute.data:
        color_element.color = (
            primary_color[0],
            primary_color[1],
            primary_color[2],
            0.5
        )

    obj.data.update()

#set the secondary custom attribute   
def set_secondary_attribute(obj):

    secondary_color = get_secondary_color(obj)

    color_attributes = obj.data.color_attributes
    color_attribute  = color_attributes.get("SecondaryColor")

    if color_attribute is None:
        color_attribute = color_attributes.new(
            name="SecondaryColor",
            type='FLOAT_COLOR',
            domain='POINT'
        )

    elif color_attribute.domain != 'POINT':
        print(f"{obj.name}: SecondaryColor exists but is not on the POINT domain")
        return

    for color_element in color_attribute.data:
        color_element.color = (
            secondary_color[0],
            secondary_color[1],
            secondary_color[2],
            0.5
        )

    obj.data.update()
    
def print_color_attributes(objs):
    for obj in objs:
        if obj.type != 'MESH':
            continue
        
        if obj is None:
            print("No active object")
        
        point_colors = obj.data.color_attributes.get("PrimaryColor")
        shadow_colors = obj.data.color_attributes.get("SecondaryColor")
        
        if point_colors is None:
            print("No PointColor attribute found")
            return
        
        elif shadow_colors is None:
            print("No ShadowColor attribute found")
        
        for i, vertex in enumerate(obj.data.vertices):
            print(
                    i,  
                    tuple(point_colors.data[i].color), 
                    tuple(shadow_colors.data[i].color)
            )

def process_all_color_data():
    objs = bpy.context.selected_objects
    
    for obj in objs:
        set_primary_attribute(obj)
        set_secondary_attribute(obj)
    
    print("color attributes succesfully set")
    
    print_color_attributes(objs)

def just_print_color_attributes():
    objs = bpy.context.selected_objects
    print_color_attributes(objs)

#process_all_color_data()
just_print_color_attributes()