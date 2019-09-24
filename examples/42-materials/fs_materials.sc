$input v_texcoord0, v_materialID

#include "../common/common.sh"

SAMPLER2DARRAY(s_texColor, 0);

uniform vec3 tint;

void main()
{
    gl_FragColor = texture2DArray(s_texColor, vec3(v_texcoord0, float(v_materialID)));
    gl_FragColor.rgb += tint;
}
