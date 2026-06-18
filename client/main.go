package main

import (
    "github.com/gofiber/fiber/v3"
    "github.com/gofiber/template/html/v3"
)

func main() {
    // Use `html/template` view engine
        engine := html.New("./views", ".html")
        app := fiber.New(fiber.Config{
            Views: engine,
        })

    // Route for static files
        app.Static("/", "./public")

    // Routes for view
        app.Get("/", func(c fiber.Ctx) error {
            return c.Render("index", fiber.Map{
                "Username": "elmyrockers",
            })
        })

    app.Listen(":3000")
}