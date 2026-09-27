import bpy
import colorsys

def set_all_ramps_to_linear_interpolation( type = 'LINEAR'):
    objs = bpy.context.selected_objects
    
    for obj in objs:
        material = obj.active_material
        node = material.node_tree.nodes['Color Ramp']
        color_ramp = node.color_ramp
        color_ramp.interpolation = type

def set_ramp_positions(position_1 = 0.0, position_2 = 1.0):
    objs = bpy.context.selected_objects
    
    for obj in objs:
        material = obj.active_material
        node = material.node_tree.nodes['Color Ramp']
        color_ramp = node.color_ramp
        elements = color_ramp.elements
        
        elements[0].position = position_1
        elements[1].position = position_2

def set_ramp_colors(color_1 = 0.0 , color_2 = 1.0):
    objs = bpy.context.selected_objects
    
    for obj in objs:
        material = obj.active_material
        node = material.node_tree.nodes['Color Ramp']
        color_ramp = node.color_ramp
        elements = color_ramp.elements
        
        elements[0].position = position_1
        elements[1].position = position_2

def set_shadow_color():
    objs = bpy.context.selected_objects
    
    for obj in objs:
        material = obj.active_material
        node = material.node_tree.nodes['Color Ramp']
        color_ramp = node.color_ramp
        color = color_ramp.elements[1].color
        shadow = color_ramp.elements[0].color
        
        #color_hsv = colorsys.rgb_to_hsv(color[0], color[1], color[2])
        
        hue, saturation, value = colorsys.rgb_to_hsv(color[0], color[1], color[2])
        
        saturation  = 1.0
        hue         -= 0.1
        value       = 0.01
        
        red, green, blue = colorsys.hsv_to_rgb(hue, saturation, value)
        
        color_ramp.elements[0].color = (red, green, blue, 1.0)
#set_ramp_positions(0.0, 1.0)
#set_all_ramps_to_linear_interpolation()

set_shadow_color()