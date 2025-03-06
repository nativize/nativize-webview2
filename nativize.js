export const check = async () => {
  console.log("Checking webview 2...");
  console.log("we check dependencies, development environments");
  console.log("we need NuGet, MSBuild, Windows SDK, etc, commands");
};

export const prepare = async () => {
  console.log("Preparing webview 2...");
  console.log("Installing NuGet packages...");

  await new Deno.Command("nuget", {
    args: [
      "restore",
      "nativize-webview2.sln",
    ],
    cwd: import.meta.dirname,
  }).spawn().status;
};

/**
 * 
 * @param {*} url 
 */
export const build = async ({url}) => {
  await new Deno.Command("msbuild", {
    args: [
      //"nativize-webview2.sln",
      `/p:NativizeURL=${url}`,
    ],
    cwd: import.meta.dirname,
  }).spawn().status;
};

export const run = async () => {
  //TODO: we need to implement some way to access several architecture directories
  await new Deno.Command(
    `${import.meta.dirname}/x64/Debug/nativize-webview2.exe`,
    {},
  ).spawn().status;
};

export const clean = async () => {
  console.log("Cleaning webview 2...");

  await new Deno.Command("msbuild", {
    args: [
      `nativize-webview2.sln`,
      "/t:Clean",
    ],
    cwd: import.meta.dirname,
  }).spawn().status;
};
