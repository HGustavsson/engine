#version 430

in vec3 fragLoc;
in vec2 fragUV;
uniform sampler2D myTexture;

void main()
{
	//gl_FragColor = vec4(sin((fragLoc.y+1)/2), sin((fragLoc.z+1)/2), sin((fragLoc.x+1)/2), 1);

	gl_FragColor = texture(myTexture, fragUV);
}