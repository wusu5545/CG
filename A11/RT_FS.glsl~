#version 120

#define NUM_PLANES 1 // 1 Ground plane
#define NUM_SPHERES 5 // Sun, Earth, Moon, Saturn

#define NUM_MAX_INTERSECTIONS (NUM_PLANES + NUM_SPHERES)
#define NUM_MAX_REFLECTIONS 8
#define NUM_MAX_REFRACTIONS 16
#define EPSILON 0.002
#define INFINITY 1000

// Object IDs
#define OBJ_ID_SUN 0
#define OBJ_ID_EARTH 1
#define OBJ_ID_MOON 2
#define OBJ_ID_SATURN 3
#define OBJ_ID_BIG_SPHERE 4
#define OBJ_ID_GROUND_PLANE NUM_SPHERES

// Uniform variables
uniform vec2 uResolution;
uniform float uFov;
uniform vec3 uUp;
uniform vec3 uDirection;
uniform vec3 uPosition;
uniform float uTime;

// XYZ - center of the sphere
// W - radius of the sphere
uniform vec4 uSphereData[NUM_SPHERES-1];

// Common data types
struct Ray {
	vec3 origin;
	vec3 direction;
};

struct IntersectionTestResult {
	bool isIntersection;
	bool isInside; // true iff the intersection happened inside an object
	float tHit;
	float epsilon; // Dynamic epsilon - required for the spikeball like object
};

struct SceneIntersectionTestResult {
	IntersectionTestResult intersection;
	int objectId;
	vec3 normal;
	vec3 hitPosition;
};

const IntersectionTestResult noIntersection = IntersectionTestResult(false, false, 0, EPSILON);
const SceneIntersectionTestResult noSceneIntersection = SceneIntersectionTestResult(noIntersection, -1, vec3(0), vec3(0));

// A very bad noise function
float pn(vec3 p) {
	vec4 a=dot(floor(p),vec3(1,57,21))+vec4(0,57,21,78);
	p=smoothstep(0,1,fract(p));
	a=mix(cos(a*a),cos((a+1)*(a+1)),p.x);
	a.xy=mix(a.xz,a.yw,p.y);
	return mix(a.x,a.y,p.z);
}

// SDF for spikeball like object (it's magic)
float f(vec3 p){
	#define R(p, a) p=cos((a))*p+sin((a))*vec2(p.y, -p.x)
	R(p.yz, .05 * uTime);
	R(p.xz, .05 * uTime  + sin(0.1 * uTime) * p.x * 0.01);
	vec3 q = p;
	p = abs(p);
	if (p.x <= max(p.z, p.y)) p = p.yzx;
	if (p.x <= max(p.z, p.y)) p = p.yzx;
	
	float b = max(max(max(
				dot(p, vec3(.577)),
				dot(p.xz, vec2(.934,.357))),
				dot(p.yx, vec2(.526,.851))),
				dot(p.xz, vec2(.526,.851))
			);
	float l = length(p);

	float d = l - 25- 0.2*(25/2)* cos(min(sqrt(1.01-b/l)*(3.1415/0.25), 3.1415));

	return (d + pn(q*4.)*0.00)*0.75;
}

// Computes the normal of the SDF defined by f() at the position p
// using central differences
vec3 g(vec3 p) {
	return normalize(
		vec3(
			f(vec3(.1,0,0)+p),
			f(vec3(0,.1,0)+p),
			f(vec3(0,0,.1)+p)
			)

		-
			 
		vec3(
			f(vec3(-.1,0,0)+p),
			f(vec3(0,-.1,0)+p),
			f(vec3(0,0,-.1)+p)
			)
	);
}

// Intersection tests
IntersectionTestResult intersectRaySphere(Ray ray, vec4 sphereData) {
	float rr = sphereData.w * sphereData.w;
	vec3 l = sphereData.xyz - ray.origin;
	float ll = dot(l, l);
	float s = dot(l, ray.direction);
	if (s < 0 && ll > rr)
		return noIntersection;
	
	float mm = ll - s*s;
	if (mm > rr)
		return noIntersection;
	
	float u = sign(ll - rr);
	float tHit = s - sqrt(rr - mm) * u;
	return IntersectionTestResult(true, u <= 0, tHit, EPSILON);

}

IntersectionTestResult intersectRaySDF(Ray ray) {
	IntersectionTestResult result = intersectRaySphere(ray, vec4(0,0,0,26.));

	// Early exit
	if (!result.isIntersection)
		return noIntersection;


	vec3 p = ray.origin;
	vec3 q = p;
	vec3 d = ray.direction;

	// Advance to the bounding sphere
	if (!result.isInside)
		p += d * result.tHit;

	// Sphere tracing
	float l = f(p);      // Distance value at position p
	float s = sign(l);
	float r = 0.0;
	float i = 0.0;       // Iteration counter 0.0 - 1.0
	float t = 1.0/128.0; // Counter increment -> 128 steps max
	float e = 0.008;     // Error threshold

	while (i < 1.0 && abs(l) > r*e && r < 60.0) {
		l = f(p);
		r += abs(l);
		p += d*l*s;
		i += t;
	}
	
	if (i >= 1.0 || r > 60.0)
		return noIntersection;
	
	return IntersectionTestResult(true, s <= 0, length(q-p), abs(l)*2.0);
}


// http://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/raycast/sld017.htm
// Ray plane intersection test
IntersectionTestResult intersectRayPlane(Ray ray, vec4 planeData) {
	float v = -dot(ray.direction, planeData.xyz);
	float tHit = (dot(ray.origin, planeData.xyz) - planeData.w) / v;
	return IntersectionTestResult(tHit > 0, false, tHit, EPSILON);
}

vec3 getNormal(int objectId, vec3 hitPosition) {
	if (objectId == OBJ_ID_SUN) {
		return g(hitPosition);
	} else if (objectId < OBJ_ID_GROUND_PLANE) { // SPHERE
		return normalize(hitPosition - uSphereData[objectId].xyz);
	} else if (objectId == OBJ_ID_GROUND_PLANE) { // PLANE
		return vec3(0,1,0);
	}
}


// Returns the closest intersection of the ray with the scene
SceneIntersectionTestResult intersectRayScene(Ray ray) {
	// Perform intersection tests
	IntersectionTestResult results[NUM_MAX_INTERSECTIONS];
	
	// Compute the intersection with the spikeball SDF
	results[0] = intersectRaySDF(ray);

	// For all spheres
	for (int i = 1; i < NUM_SPHERES-1; ++i)
		results[i] = intersectRaySphere(ray, uSphereData[i]);
	results[OBJ_ID_BIG_SPHERE] = intersectRaySphere(ray, vec4(0,0,0,200));

	// For the ground plane
	results[NUM_SPHERES] = intersectRayPlane(ray, vec4(0,1,0,-30));
	

	// Find the closest intersection along the ray
	int objectId = -1;
	float tHitMin = INFINITY;
	for (int i = 0; i < NUM_MAX_INTERSECTIONS; ++i) {
		if (results[i].isIntersection && results[i].tHit < tHitMin) {
			objectId = i;
			tHitMin = results[i].tHit;
		}
	}
	
	if (objectId == -1)
		return noSceneIntersection;
		
	// Compute normal and the position of the intersection
	vec3 hitPosition = ray.origin + results[objectId].tHit * ray.direction;
	vec3 normal = getNormal(objectId, hitPosition);

	// Return the closest intersection
	return  SceneIntersectionTestResult(results[objectId], objectId, normal, hitPosition);
}

// Procedural checkerboard texture
vec4 smoothCheckerboard(vec2 texCoord) {
	float fq = 0.5;
	vec2 w = fwidth(texCoord.xy);
	vec2 fu = w * fq * 2;
	float fum = max(fu.x, fu.y);
	vec2 cp = fract(texCoord.xy * fq);

	vec2 pp = smoothstep(vec2(.5), fu + vec2(.5), cp) +	(1.0 - smoothstep(vec2(0.0), fu, cp));
	vec4 c0 = vec4(1);
	vec4 c1 = vec4(0);
	vec4 ca = c0 * .5 + c1 * .5;

	vec4 cc = mix(c0, c1, pp.x * pp.y + (1 - pp.y) * (1 - pp.x));
	cc = mix(cc, ca, smoothstep(.125, .75, fum));
	return cc;
}

// Computes the local color at a hitpoint
vec4 getColor(SceneIntersectionTestResult result, Ray ray) {
	vec3 toLightP = normalize(vec3(0,100,0) - result.hitPosition);
	vec3 N = result.normal;
	float nDotLP = max(0, dot(N, toLightP));

	vec4 baseColor = vec4(1.0,1.0,1.0,1.0);

	if (result.objectId == OBJ_ID_SUN) {
		baseColor = vec4(0,0,0.8,1.);
	} else if (result.objectId == OBJ_ID_EARTH) {
		baseColor = vec4(.9,.7,1.,1.);
	} else if (result.objectId == OBJ_ID_MOON) {
		baseColor = vec4(2.0,.3,0.2,1.0);
	} else if (result.objectId == OBJ_ID_SATURN) {
		baseColor = vec4(1.,1.,0.3,1.);
	} else if (result.objectId == OBJ_ID_BIG_SPHERE) {
		baseColor = vec4(0.5,0.7,1.0,1.0);
	}

	vec4 color = result.objectId == OBJ_ID_SUN ? 0.125 * (nDotLP) * baseColor : 0.3 * (nDotLP) * baseColor;
	color += pow(max(dot(result.normal ,normalize(toLightP - ray.direction)), 0), 128.);

	if (result.objectId == OBJ_ID_BIG_SPHERE) {
		float yMax = 5.;
		float x = 1. - clamp((result.hitPosition.y+30), 0., yMax) / yMax;
		color = mix(vec4(0.8), vec4(0.3), x*x);
	}

	if (result.objectId == OBJ_ID_GROUND_PLANE)
		color *= (smoothCheckerboard(result.hitPosition.xz*0.05)*.5+.5);

	return color;
}

// TODO: c) Compute the transmittance
// See: http://www.bramz.net/data/writings/reflection_transmission.pdf
// Hint: You might have to change something in order to handle total internal reflections
// properly in the refraction computation loop
float transmittance(vec3 i, vec3 n, float eta1, float eta2) {
	float eta = eta1 / eta2;
	float cosI = -dot(n,i);
	float sinT2 = eta * eta * (1.0 - cosI * cosI);
	if (sinT2 > 1.0) return 1.0 ; //TIR
	float cosT = sqrt(1.0 - sinT2);
	float r0rth = (eta1 * cosI - eta2 * cosT) / (eta1 * cosI + eta2 * cosT);
	float rPar = (eta2 * cosI - eta1 * cosT) / (eta2 * cosI + eta1 * cosT);
	return 1.0 - (r0rth * r0rth + rPar * rPar) / 2.0;
}

// Entry point
void main() {
	// Camera setup
	vec3 nD = normalize(uDirection.xyz);
	vec3 nS = normalize(cross(nD, uUp.xyz));
	vec3 nUp = cross(nS, nD);
	mat3 M = mat3(nS, nUp, -nD);


	// Setup primary ray
	Ray primaryRay;
	primaryRay.origin = uPosition.xyz;
	primaryRay.direction =  M *
		normalize(vec3((gl_FragCoord.xy-vec2(.5*uResolution.xy))/uResolution.y,-.5/tan(.5*3.1415*uFov/180.)));

	// Intersect primary ray with the scene
	SceneIntersectionTestResult r = intersectRayScene(primaryRay);
	// Backup the intersection of the primary ray with the scene
	SceneIntersectionTestResult rr = r;

	// Shade the background white in case nothing was hit
	if (r.objectId == -1) {
		gl_FragColor = vec4(1.0,1.0,1.0,1.0);
		return;
	}

	// Refraction indices
	const float etaOutside = 1.0, etaInside = 1.1;

	// Retrieve color at the first hitpoint
	vec4 primaryRayColor = getColor(r, primaryRay);

	// Compute reflections for spikeball & earth
	Ray reflectionRay = primaryRay;
	vec4 pathThroughput = vec4(2.0);	// The initial value of the "throughtput"
	vec4 reflectedColor = vec4(0.0);	// Result color of the reflection loop
		
	for (int i = 0; i < NUM_MAX_REFLECTIONS && (r.objectId == OBJ_ID_SUN || r.objectId == OBJ_ID_EARTH); ++i) {
		if (r.objectId == OBJ_ID_SUN) {
			// TODO c) Modify the pathThroughput with the reflectance (1 - transmittance) (sun/spikeball)
			pathThroughput *= 1.0 - transmittance(normalize(reflectionRay.direction),r.normal,etaOutside,etaInside);
		} else if (r.objectId == OBJ_ID_EARTH) {
			// Modify the pathTroughput by the color at the hitpoint (Earth)
			pathThroughput *= getColor(r, reflectionRay);
		}
	
		// Setup the reflection ray & apply epsilon correction
		reflectionRay = Ray(r.hitPosition + r.normal * r.intersection.epsilon, reflect(reflectionRay.direction, r.normal));
		r = intersectRayScene(reflectionRay);
		if (r.objectId == -1) {
			break;
		}
	
		// In case neither the sun/spikeball nor the earth are hit
		// add the color weighted by the pathTroughput to the refracted color
		if (r.objectId != OBJ_ID_SUN && r.objectId != OBJ_ID_EARTH) {
			reflectedColor += pathThroughput * getColor(r, reflectionRay);
		}
	}

	// Compute refraction for sun/spikeball
	// Reset r to the intersection of the primary ray with the scene
	r = rr;
	Ray refractionRay = primaryRay;
	pathThroughput = vec4(1.0); 		// The initial value of the "throughtput"
	vec4 refractedColor = vec4(0.0);	// Result color of the refraction loop
	for (int i = 0; i < NUM_MAX_REFRACTIONS && r.objectId == OBJ_ID_SUN; ++i) {
		// s == -1 in case the hit occurred inside an object, s == 1 otherwise
		float s = r.intersection.isInside ? -1.0 : 1.0;
		
		// TODO: a) Select the etas depending on where the intersection with the scene occurred
		float eta1 = etaOutside;
		float eta2 = etaInside;
		if (s == -1){
			eta1 = etaInside;
			eta2 = etaOutside;
		}

		// TODO: c) Modify the pathThroughput with the transmittance
		pathThroughput *= transmittance(normalize(refractionRay.direction),s*r.normal,eta1,eta2);

		// TODO: a) Calculate the direction of the refracted ray
		// HINT: You can use the GLSL function refract(I,N)
		vec3 refractedDirection = normalize(refract(normalize(refractionRay.direction),s*r.normal,eta1/eta2));
		
		// TODO: b) Handle total internal reflection (TIR) - reflect the ray if neccessary
		// HINT: Use the GLSL function reflect(I,N)
		float TIRfactor = 1.0;	// -1.0 on total internal reflection, 1.0 otherwise
		if (refractedDirection == vec3(0,0,0))
			TIRfactor = -1.0;

		// TODO: a) Setup the new refraction (reflection in case of TIR) ray & apply epsilon correction
		// TODO: b) Take TIRfactor into account
		if(TIRfactor == -1.0)
			refractionRay = Ray(r.hitPosition + s*r.normal*r.intersection.epsilon, reflect(refractionRay.direction,r.normal));
		else
			refractionRay = Ray(r.hitPosition - s*r.normal*r.intersection.epsilon,refractedDirection);
		
		// Intersect the ray with the scene
		r = intersectRayScene(refractionRay);
		if (r.objectId == -1)
			break;
	
		// TODO: d) In case the intersection occurred inside the spikeball (OBJ_ID_SUN)
		// modify the pathThroughtput by applying the Beer-Lambert law
		// Use: exp(-alpha * l) where alpha is 0.05 * vec4(0.7, 0.1, 0.3, 1.0)
		// and l == tHit.
		// See: http://en.wikipedia.org/wiki/Beer%E2%80%93Lambert_law
		pathThroughput *= exp(-0.05 * vec4(0.7,0.1,0.3,1.0) * r.intersection.tHit);
		// _otherwise_ add the color weighted by the pathTroughput to the refracted color
		refractedColor += getColor(r, refractionRay) * pathThroughput;
	
	}
	
	// Compute the final color of the fragment
	gl_FragColor = clamp(primaryRayColor + reflectedColor + refractedColor, 0., 1.);
}