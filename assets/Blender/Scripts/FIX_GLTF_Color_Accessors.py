import json
import struct
import sys

JSON_CHUNK_TYPE = 0x4E4F534A

input_path = sys.argv[1]
output_path = sys.argv[2]

#---------------------------------------------------------- read GLB

with open(input_path, "rb") as file:
    data = file.read()

magic, version, total_length = struct.unpack_from(
    "<4sII",
    data,
    0
)

if magic != b"glTF":
    raise RuntimeError("The input file is not a GLB file")

# Preserve every GLB chunk.
chunks = []

offset = 12

while offset < len(data):
    chunk_length, chunk_type = struct.unpack_from(
        "<II",
        data,
        offset
    )

    offset += 8

    chunk_data = data[
        offset:
        offset + chunk_length
    ]

    chunks.append(
        [chunk_type, chunk_data]
    )

    offset += chunk_length

#---------------------------------------------------------- find JSON

json_chunk = None

for chunk in chunks:
    if chunk[0] == JSON_CHUNK_TYPE:
        json_chunk = chunk
        break

if json_chunk is None:
    raise RuntimeError("GLB does not contain a JSON chunk")

document = json.loads(
    json_chunk[1].decode("utf-8")
)

accessors = document.get("accessors", [])
buffer_views = document.get("bufferViews", [])

#---------------------------------------------------- repair accessors

changed = 0

for mesh_index, mesh in enumerate(document.get("meshes", [])):
    for primitive_index, primitive in enumerate(
        mesh.get("primitives", [])
    ):
        for semantic, accessor_index in primitive.get(
            "attributes",
            {}
        ).items():
            if not semantic.startswith("COLOR_"):
                continue

            accessor = accessors[accessor_index]

            if accessor.get("componentType") != 5126:
                continue

            if accessor.get("type") != "VEC4":
                continue

            view = buffer_views[accessor["bufferView"]]

            count = accessor["count"]
            rgb_size = count * 3 * 4
            rgba_size = count * 4 * 4
            actual_size = view["byteLength"]

            if actual_size == rgb_size:
                accessor["type"] = "VEC3"
                changed += 1

                print(
                    f"{semantic}: accessor {accessor_index} "
                    f"changed from VEC4 to VEC3 "
                    f"({actual_size} bytes)"
                )

            elif actual_size >= rgba_size:
                print(
                    f"{semantic}: already contains valid VEC4 data"
                )

            else:
                print(
                    f"{semantic}: unexpected buffer size "
                    f"{actual_size}; left unchanged"
                )

if changed == 0:
    raise RuntimeError(
        "No malformed floating-point colour accessors were found"
    )

#----------------------------------------------------- replace JSON chunk

new_json = json.dumps(
    document,
    separators=(",", ":")
).encode("utf-8")

while len(new_json) % 4 != 0:
    new_json += b" "

json_chunk[1] = new_json

#-------------------------------------------------------- rebuild GLB

new_total_length = 12

for chunk_type, chunk_data in chunks:
    new_total_length += 8 + len(chunk_data)

with open(output_path, "wb") as file:
    file.write(
        struct.pack(
            "<4sII",
            b"glTF",
            version,
            new_total_length
        )
    )

    for chunk_type, chunk_data in chunks:
        file.write(
            struct.pack(
                "<II",
                len(chunk_data),
                chunk_type
            )
        )

        file.write(chunk_data)

print(f"Saved repaired GLB: {output_path}")

#python3 \                                                                                               
#   ../assets/Blender/Scripts/Fix_GLTF_Color_Accessors.py \
#   ../assets/Models/birb/red_shadow_test.glb \
#   ../assets/Models/birb/red_shadow_test_fixed.glb
