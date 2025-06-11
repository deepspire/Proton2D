local IS_WINDOWS = is_plat("windows")
local IS_LINUX = is_plat("linux")

if not IS_WINDOWS then
	add_requires("libsdl3", "libsdl3_ttf", "libsdl3_image")
end

set_config("mode", "release")
target("Proton2D")

do
	set_kind("binary")
	add_files("src/*.cc")
	add_files("src/proton/*.cc")
	add_includedirs("ext")
	if IS_WINDOWS then
		
		add_includedirs("C:/SDL3/x86_64-w64-mingw32/include")
		add_linkdirs("C:/SDL3/x86_64-w64-mingw32/lib")

		add_includedirs("C:/SDL3_ttf/x86_64-w64-mingw32/include")
		add_linkdirs("C:/SDL3_ttf/x86_64-w64-mingw32/lib")

		add_includedirs("C:/SDL3_image/x86_64-w64-mingw32/include")
		add_linkdirs("C:/SDL3_image/x86_64-w64-mingw32/lib")
	end

	add_links("SDL3", "SDL3_ttf", "SDL3_image")

	if IS_WINDOWS then
		--add_ldflags("-mwindows") -- можно убрать если нужно чекать консоль
	end
end
