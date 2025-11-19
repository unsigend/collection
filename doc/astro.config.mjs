// @ts-check
import { defineConfig } from "astro/config";
import starlight from "@astrojs/starlight";

// https://astro.build/config
export default defineConfig({
    // GitHub Pages configuration
    // Update these values to match your repository:
    // - If repo is "username/collection", use: site: "https://username.github.io/collection/", base: "/collection/"
    // - If deploying to user page (username.github.io), use: site: "https://username.github.io", base: "/"
    site: process.env.GITHUB_PAGES_URL || "https://<username>.github.io/collection/",
    base: process.env.GITHUB_PAGES_BASE || "/collection/",
    output: "static",
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
