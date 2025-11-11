// @ts-check
import { defineConfig } from "astro/config";
import starlight from "@astrojs/starlight";

// https://astro.build/config
export default defineConfig({
    integrations: [
        starlight({
            title: "collection",
            description:
                "A generic data structure and algorithms library for modern C",
            customCss: ["./src/custom.css"],
            social: [
                { icon: "github", label: "GitHub", href: "https://github.com" },
            ],
            sidebar: [
                {
                    label: "Guide",
                    items: [
                        { label: "Introduction", slug: "introduction" },
                    ],
                },
                {
                    label: "Collection",
                    autogenerate: { directory: "collection" },
                },
                {
                    label: "Algorithms",
                    autogenerate: { directory: "algorithms" },
                },
            ],
        }),
    ],
});
