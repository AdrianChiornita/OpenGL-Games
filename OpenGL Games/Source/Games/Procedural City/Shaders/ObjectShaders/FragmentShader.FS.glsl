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
uniform Light spots[4];
uniform Light light;
uniform Material material;
uniform vec3 eye_position;

layout(location = 0) out vec4 out_color;
void main()
{
	float kc = 1.0f;
	float kl = 0.05f;
	float kq = 0.0f;

	vec3  color = vec3(0);

	for (int index = 0; index < 4; ++index) 
	{
		vec3 object_color = texture2D(texmap, fragment_texture_coordinate).rgb;
		float distancetosource = length( spots[index].position - world_position );
		vec3 L = normalize( spots[index].position - world_position );
		vec3 V = normalize( eye_position - world_position );
		vec3 H = normalize( L + V );
		float angle = max( dot( world_normal, L ), 0.0f );
		float cut_off = radians(30.0f);
        float spot_light = dot(-L, spots[index].direction);

		float attenuation;
        if (spot_light > cos(cut_off))
        {
            attenuation = (spot_light - cos(cut_off))/(1 - cos(cut_off));
            attenuation = pow(attenuation, 2);
        }else{
            attenuation = 1;  
        }
		
		//define emisive_light
		vec3  emisive_light = material.Ke * vec3(1);
	
		// define ambient light component

		vec3  ambient_light = material.Ka * spots[index].ambient_color * object_color;

	
		// compute diffuse light component
		vec3  diffuse_light = material.Kd
							* spots[index].diffuse_color
							* angle
							* object_color;
		// compute specular light component
		vec3 specular_light = vec3(0);
	
		if (angle > 0)
		{
			 specular_light = material.Ks 
							* spots[index].specular_color 
							* pow ( max( dot( world_normal, H ), 0 ), material.shininess ) 
							* object_color;
		}
		
		// compute light
		vec3  partial_color = emisive_light
					+ attenuation * (
					  ambient_light 
					+ diffuse_light 
					+ specular_light );

		color += partial_color;
	}

	vec3 object_color = texture2D(texmap, fragment_texture_coordinate).rgb;
	float distancetosource = length(light.position - world_position );
	vec3 L = normalize( light.position - world_position );
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );
	float angle = max( dot( world_normal, L ), 0.0f );
	float attenuation = 1 / (kc + kl * distancetosource + kq * distancetosource * distancetosource);  


	//define emisive_light
	vec3  emisive_light = material.Ke * vec3(1.0f);
	
	// define ambient light component

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

	color += emisive_light
				+ attenuation * (
				  ambient_light 
				+ diffuse_light 
				+ specular_light );
	
	//send color light output to fragment shader
	out_color = vec4(color, 1.0f);
} 