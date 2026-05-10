using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using TodoApp.Data;
using TodoApp.Models;

namespace TodoApp.Controllers;

public class TodoController : Controller
{
    private readonly AppDbContext _context;

    public TodoController(AppDbContext context)
    {
        _context = context;
    }

    public async Task<IActionResult> Index()
    {
        var items = await _context.TodoItems
            .OrderByDescending(t => t.CreatedAt)
            .ToListAsync();
        return View(items);
    }

    [HttpPost]
    public async Task<IActionResult> Create(string title)
    {
        if (!string.IsNullOrWhiteSpace(title))
        {
            _context.TodoItems.Add(new TodoItem { Title = title });
            await _context.SaveChangesAsync();
        }
        return RedirectToAction(nameof(Index));
    }

    [HttpPost]
    public async Task<IActionResult> Toggle(int id)
    {
        var item = await _context.TodoItems.FindAsync(id);
        if (item != null)
        {
            item.IsCompleted = !item.IsCompleted;
            await _context.SaveChangesAsync();
        }
        return RedirectToAction(nameof(Index));
    }

    [HttpPost]
    public async Task<IActionResult> Delete(int id)
    {
        var item = await _context.TodoItems.FindAsync(id);
        if (item != null)
        {
            _context.TodoItems.Remove(item);
            await _context.SaveChangesAsync();
        }
        return RedirectToAction(nameof(Index));
    }
}