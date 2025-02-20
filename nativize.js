export const check = async () => {
  console.log("Checking webview 2...");
  console.log("we check dependencies, development environments");
  console.log("we need NuGet, MSBuild, Windows SDK, etc, commands");
};

export const prepare = async () => {
  console.log("Preparing webview 2...");
  console.log("Installing NuGet packages...");

  try {
    await new Deno.Command("nuget", {
      args: [
        "restore",
        `${import.meta.dirname}/nativize-webview2.sln`,
      ],
    }).spawn().status;
  } catch (error) {
    console.error(error);
  }
};

export const build = async () => {
  //TODO: get rid of this
  console.log("Building webview 2...");

  try {
    await new Deno.Command("msbuild", {
      args: [
        `${import.meta.dirname}/nativize-webview2.sln`,
      ],
    }).spawn().status;
  } catch (error) {
    console.error(error);
  }
};

export const run = async () => {
  //TODO: get rid of this
  console.log("Running webview 2...");

  try {
    //TODO: we need to implement some way to access several architecture directories
    await new Deno.Command(
      `${import.meta.dirname}/x64/Debug/nativize-webview2.exe`,
      {},
    ).spawn().status;
  } catch (error) {
    console.error(error);
  }
};


export const clean = async () => {
  console.log("Cleaning webview 2...");

  try {
    await new Deno.Command("msbuild", {
      args: [
        `${import.meta.dirname}/nativize-webview2.sln`,
        "/t:Clean",
      ],
    }).spawn().status;
  } catch (error) {
    console.error(error);
  }
}