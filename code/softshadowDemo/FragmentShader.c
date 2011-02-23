uniform sampler2DShadow ShadowMap;

varying vec4 ShadowCoord;

// This define the value to move one pixel left or right
uniform float xPixelOffset ;

// This define the value to move one pixel up or down
uniform float yPixelOffset ;

float lookup( vec2 offSet)
{
	// Values are multiplied by ShadowCoord.w because shadow2DProj does a W division for us.
	return shadow2DProj(ShadowMap, ShadowCoord + vec4(offSet.x * xPixelOffset * ShadowCoord.w, offSet.y * yPixelOffset * ShadowCoord.w, 0.05, 0.0) ).w;
}

void main()
{	

	// Used to lower moiré pattern and self-shadowing
	//shadowCoordinateWdivide.z += ;
	
	
	float shadow ;
	
	// Avoid counter shadow
	if (ShadowCoord.w > 1.0)
	{
		// Simple lookup, no PCF
					//shadow = lookup(vec2(0.0,0.0));
		




		// 8x8 kernel PCF
					
					float x,y;
					for (y = -3.5 ; y <=3.5 ; y+=1.0)
						for (x = -3.5 ; x <=3.5 ; x+=1.0)
							shadow += lookup(vec2(x,y));
					
					shadow /= 64.0 ;
					
		




		// 8x8 PCF wide kernel (step is 10 instead of 1)
					/*
					float x,y;
					for (y = -30.5 ; y <=30.5 ; y+=10.0)
						for (x = -30.5 ; x <=30.5 ; x+=10.0)
							shadow += lookup(vec2(x,y));
					
					shadow /= 64.0 ;
					*/
	



		// 4x4 kernel PCF
		/*
		float x,y;
		for (y = -1.5 ; y <=1.5 ; y+=1.0)
			for (x = -1.5 ; x <=1.5 ; x+=1.0)
				shadow += lookup(vec2(x,y));
		
		shadow /= 16.0 ;
		*/
		


		// 4x4  PCF wide kernel (step is 10 instead of 1)
					/*
					float x,y;
					for (y = -10.5 ; y <=10.5 ; y+=10.0)
						for (x = -10.5 ; x <=10.5 ; x+=10.0)
							shadow += lookup(vec2(x,y));
					
					shadow /= 16.0 ;
					*/
		
		// 4x4  PCF dithered
					/*
					// use modulo to vary the sample pattern
					vec2 o = mod(floor(gl_FragCoord.xy), 2.0);
				
					shadow += lookup(vec2(-1.5, 1.5) + o);
					shadow += lookup(vec2( 0.5, 1.5) + o);
					shadow += lookup(vec2(-1.5, -0.5) + o);
					shadow += lookup(vec2( 0.5, -0.5) + o);
					shadow *= 0.25 ;
					*/
	}
  	gl_FragColor =	  (shadow+0.2) * gl_Color;
  
}
