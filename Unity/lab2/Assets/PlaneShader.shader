// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'

// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'

Shader "Custom/PlaneShader"
{
    Properties
    {
        _Color ("Color", Color) = (1,1,1,1)
        _MainTex ("Albedo (RGB)", 2D) = "white" {}
        _NormalMap("Normal Map (RGB)", 2D) = "white" {}
        _Glossiness ("Smoothness", Range(0,1)) = 0.5
        _Metallic ("Metallic", Range(0,1)) = 0.0

        // phong
        _ambient_strength ("ambient_strength", Range(0,1)) = 0.0
        _specular_strength ("specular_strength", Range(0,1)) = 0.0
    }
 
    SubShader
    {
        Tags { "RenderType" = "Opaque" }
        LOD 200

        CGPROGRAM
        // Physically based Standard lighting model, and enable shadows on all light types
        #pragma surface surf Standard fullforwardshadows

        // Use shader model 3.0 target, to get nicer looking lighting
        #pragma target 3.0

        sampler2D _MainTex;
        sampler2D _NormalMap;

        struct Input
        {
            fixed2 uv_MainTex;
            fixed2 uv_NormalMap;

            fixed4 pos : SV_POSITION;

            fixed3 normal : NORMAL;
            fixed3 tangent : TANGENT;
        };

        half _Glossiness;
        half _Metallic;
        fixed4 _Color;

        // phong
        half _ambient_strength;
        half _specular_strength;

        // Add instancing support for this shader. You need to check 'Enable Instancing' on materials that use the shader.
        // See https://docs.unity3d.com/Manual/GPUInstancing.html for more information about instancing.
        // #pragma instancing_options assumeuniformscaling
        UNITY_INSTANCING_BUFFER_START(Props)
            // put more per-instance properties here
        UNITY_INSTANCING_BUFFER_END(Props)

        // Computes world space view direction
        fixed3 world_space_view_dir( in float4 v )
        {
            return _WorldSpaceCameraPos.xyz - mul(unity_ObjectToWorld, v).xyz;
        }

        fixed3 unpack_normal(sampler2D map, fixed2 uv)
        {
            fixed4 packed = tex2D(map, uv);
            fixed4 unpacked = 2 * packed - 1;
            // common convention for normal maps is to store the up direction in the Z component
            // in Unity Y - up
            return normalize(unpacked.xzy);
        }

        fixed4 phong(Input IN)
        {
            // light
            fixed4 light_color = _LightColor0;
            fixed3 light_dir = normalize(_WorldSpaceLightPos0);

            // ambient
            fixed4 ambient = _ambient_strength * light_color;

            // diffuse
            fixed3 n = unpack_normal(_NormalMap, IN.uv_NormalMap);
            float d = max(dot(n, light_dir), 0.0);
            fixed4 diffuse = d * light_color;

            // specular
            fixed3 view_dir = normalize(world_space_view_dir(IN.pos));
            fixed3 reflect_dir = reflect(-light_dir, n);
            float s = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
            fixed4 specular = _specular_strength * s * light_color;

            // object color
            fixed4 albedo = tex2D(_MainTex, IN.uv_MainTex) * _Color;

            return (ambient + specular + diffuse) * albedo;
        }

        void surf (Input IN, inout SurfaceOutputStandard o)
        {
            // Albedo comes from a texture tinted by color
            fixed4 p = phong(IN);
            o.Albedo = p.rgb;
            o.Alpha = p.a;

            // Metallic and smoothness come from slider variables
            o.Metallic = _Metallic;
            o.Smoothness = _Glossiness;

            // WORK begin {
            // // Albedo comes from a texture tinted by color
            // fixed4 albedo = tex2D (_MainTex, IN.uv_MainTex) * _Color;
            // o.Albedo = albedo.rgb;
            // o.Alpha = albedo.a;
            // o.Normal = UnpackNormal(tex2D(_NormalMap, IN.uv_NormalMap));
            // // Metallic and smoothness come from slider variables
            // o.Metallic = _Metallic;
            // o.Smoothness = _Glossiness;
            // WORK end }
        }
        ENDCG
    }
    FallBack "Diffuse"
}
