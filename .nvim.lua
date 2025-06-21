-- put the following line in your neovim configuration
-- vim.o.exrc = true
vim.keymap.set("n", "<leader>ma", "<CMD>wa<CR><CMD>make! analyzer<CR><CMD>6copen<CR>", { desc = "[M]ake [A]nalyzer" })
vim.keymap.set("n", "<leader>md", "<CMD>wa<CR><CMD>make! debug<CR><CMD>6copen<CR>",    { desc = "[M]ake [D]ebug" })
vim.keymap.set("n", "<leader>mr", "<CMD>wa<CR><CMD>make! release<CR><CMD>6copen<CR>",  { desc = "[M]ake [R]elease" })
