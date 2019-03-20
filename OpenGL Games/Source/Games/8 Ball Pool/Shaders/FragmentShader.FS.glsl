#version 330

struct Light {
	vec3 position;
	vec3 emisive_color;
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;
	vec3 direction;
};

struct Material {
	float Ke;
	float Ka;
	float Kd;
	float Ks;
	int shininess;
};

in vec2 fragment_texture_coordinate;
in vec3 world_position;
in vec3 world_normal;

uniform sampler2D texmap;
uniform Light light;
uniform Material material;
uniform vec3 eye_position;

layout(location = 0) out vec4 out_color;

void main()
{
	float kc = 1.0f, kl = 0.05f, kq = 0.0f;

	vec3 object_color = texture2D(texmap, fragment_texture_coordinate).rgb;
	float distancetosource = length( light.position - world_position );

	vec3 L = normalize( light.position - world_position );
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );
	
	float angle = max( dot( world_normal, L ), 0.0f );
	float attenuation = 1 / (kc + kl * distancetosource + kq * distancetosource * distancetosource);  


	// compute emisive_light
	vec3  emisive_light = material.Ke * vec3(1);
	
	// compute ambient light component
	vec3  ambient_light = material.Ka * light.ambient_color * object_color;

	// compute diffuse light component
	vec3  diffuse_light = material.Kd
						* light.diffuse_color
						* angle
						* object_color;
	
	// compute specular light component
	vec3 specular_light = vec3(0);
	
	if (angle > 0)
	{
		 specular_light = material.Ks 
						* light.specular_color 
						* pow ( max( dot( world_normal, H ), 0 ), material.shininess ) 
						* object_color;
	}
	
	// compute light
	vec3  color = emisive_light
				+ attenuation * (
				  ambient_light 
				+ diffuse_light 
				+ specular_light );

	// send color light output to fragment shader
	out_color = vec4(color, 1.0f);
} 