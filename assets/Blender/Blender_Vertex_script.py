obj = bpy.context.object

#Create a color_attribute
point_colors = obj.data.color_attributes.new(
    name="PointColor",
    type='FLOAT_COLOR',
    domain='POINT'
)

#read the color of each vertex
for i, v in enumerate(obj.data.vertices):
    color = tuple(point_colors.data[i].color)
    print(i, tuple(v.co), color)

#set pallete colors
palette = [
    (1,0,0,1),      # Group 0
    (0,1,0,1),      # Group 1
    (0,0,1,1),      # Group 2
    (1,1,0,1),      # Group 3
    (1,0,1,1),      # Group 4
    (0,1,1,1),      # Group 5
    (1,1,1,1),      # Group 6
    (0.5,0.5,0.5,1) # Group 7
]

#apply the pallet colors to each vertex
for v in obj.data.vertices:
    group_index = v.groups[0].group
    point_colors.data[v.index].color = palette[group_index]

#print each vertex and it's group name and weights...
for v in obj.data.vertices:
     print(f"Vertex {v.index}")
     for g in v.groups:
         print(
             "  Group:",
             obj.vertex_groups[g.group].name,
             "Weight:",
             g.weight
         )

#Group colors by name
group_colors = {
     "Red":    (1.0, 0.0, 0.0, 1.0),
     "Blue":   (0.0, 0.0, 1.0, 1.0),
     "Green":  (0.0, 1.0, 0.0, 1.0),
     "Yellow": (1.0, 1.0, 0.0, 1.0),
     "Purple": (1.0, 0.0, 1.0, 1.0),
     "Orange": (1.0, 0.5, 0.0, 1.0),
     "White":  (1.0, 1.0, 1.0, 1.0),
     "Black":  (0.0, 0.0, 0.0, 1.0),
}

#set each point color to specific color
point_colors = obj.data.color_attributes["PointColor"]

red = (1.0, 0.0, 0.0, 1.0)

for color in point_colors.data:
    color.color = red
    
#print point color values
point_colors = obj.data.color_attributes["PointColor"]

for i in range(len(point_colors.data)):
    print(i, tuple(point_colors.data[i].color))

#set them all to red
for color in point_colors.data:
    color.color = (1.0, 0.0, 0.0, 1.0)
    


# ----------------------------------To automate the whole assignment
obj = bpy.context.object

#Create a color_attribute
if "PointColor" in obj.data.color_attributes:
    point_colors = obj.data.color_attributes["PointColor"]
else:
    point_colors = obj.data.color_attributes.new(
        name="PointColor",
        type='FLOAT_COLOR',
        domain='POINT'
    )

#Set the group Palletes
palette = {
    "Group.001": (1, 0, 0, 1),
    "Group.002": (0, 1, 0, 1),
    "Group.003": (0, 0, 1, 1),
    "Group.004": (1, 1, 0, 1),
    "Group.005": (1, 0, 1, 1),
    "Group.006": (0, 1, 1, 1),
    "Group.007": (1, 1, 1, 1),
    "Group.008": (0.5, 0.5, 0.5, 1),
}

for vertex in obj.data.vertices:
    if not vertex.groups:
        continue
    group_name = obj.vertex_groups[vertex.groups[0].group].name
    point_colors.data[vertex.index].color = palette[group_name]