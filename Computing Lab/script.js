let emails = [];
let filtered = [];
let currentPage = 1;
const perPage = 20;
let readEmails = JSON.parse(localStorage.getItem("readEmails") || "[]");

async function loadEmails() {
  try {
    const res = await fetch("emails.json");
    emails = await res.json();
    filtered = emails;
    renderList();
  } catch {
    document.getElementById("emailList").innerHTML =
      "<p style='color:red;padding:10px;'>Could not load emails.json</p>";
  }
}

function renderList() {
  const list = document.getElementById("emailList");
  list.innerHTML = "";

  const start = (currentPage - 1) * perPage;
  const pageItems = filtered.slice(start, start + perPage);

  if (!pageItems.length) {
    list.innerHTML = "<p style='padding:10px;'>No emails found</p>";
  }

  pageItems.forEach((e, idx) => {
    const div = document.createElement("div");
    div.className = "email-item";

    const avatar = (e.from || "X")[0].toUpperCase();
    const isRead = readEmails.includes(e.id || (start + idx));

    div.innerHTML = `
      <div class="email-avatar">${avatar}</div>
      <div class="email-content">
        <div class="email-header">
          <span>${e.from || "Unknown"}</span>
          <span>${formatDate(e.date)}</span>
        </div>
        <div class="email-subject ${isRead ? "" : "unread"}">${e.subject || "(No Subject)"}</div>
        <div class="email-snippet">${(e.body || "").replace(/<[^>]+>/g, "").slice(0, 60)}...</div>
      </div>
    `;

    div.addEventListener("click", () => showEmail(e, start + idx));
    list.appendChild(div);
  });


  document.getElementById("pageInfo").innerText =
    `Page ${currentPage} of ${Math.ceil(filtered.length / perPage)}`;
}


function showEmail(email, index) {
  const subjectEl = document.getElementById("emailSubject");
  const metaEl = document.getElementById("emailMeta");
  const bodyEl = document.getElementById("emailBody");

  // Subject
  subjectEl.innerText = email.subject || "(No Subject)";

  // Meta Info
  metaEl.innerHTML = `
    <div class="meta-box">
      <strong>From:</strong> ${email.from || "Unknown"}<br>
      <strong>To:</strong> ${email.to || "Unknown"}<br>
      <strong>Date:</strong> ${formatDate(email.date)}
    </div>
    <div class="divider"></div>
  `;

  let bodyContent = "";

  // Prefer HTML body if available
  if (email.htmlBody) {
    bodyContent = email.htmlBody;
  } else if (email.body) {
    // Plain text body -> convert to paragraphs
    bodyContent = email.body
      .split(/\n{2,}/)
      .map(paragraph => `<p>${paragraph.trim()}</p>`)
      .join('<hr class="body-divider">');
  } else {
    bodyContent = "<i>(No body)</i>";
  }
  

  bodyEl.innerHTML = `
    <div class="mail-body-container">
      <div class="mail-body-content">
        ${bodyContent}
      </div>
    </div>
  `;

  // Fix image display
  bodyEl.querySelectorAll("img").forEach(img => {
    img.style.maxWidth = "100%";
    img.style.height = "auto";
    img.style.display = "block";
    img.style.margin = "10px auto";
    if (img.width < 20 && img.height < 20) {
      img.style.display = "none";
    }
  });

  // Fix links
  bodyEl.querySelectorAll("a").forEach(link => {
    link.style.color = "#1a73e8";
    link.style.wordBreak = "break-word";
    link.style.textDecoration = "none";
    link.style.fontWeight = "500";
    link.addEventListener("mouseover", () => (link.style.textDecoration = "underline"));
    link.addEventListener("mouseout", () => (link.style.textDecoration = "none"));
  });

  // Mark as read
  if (!readEmails.includes(email.id || index)) {
    readEmails.push(email.id || index);
    localStorage.setItem("readEmails", JSON.stringify(readEmails));
    renderList();
  }
}


function formatDate(dateStr) {
  if (!dateStr) return "";
  const d = new Date(dateStr);
  return isNaN(d.getTime()) ? dateStr : d.toLocaleString();
}

// Pagination
document.getElementById("prevPage").addEventListener("click", () => {
  if (currentPage > 1) { currentPage--; renderList(); }
});
document.getElementById("nextPage").addEventListener("click", () => {
  if (currentPage * perPage < filtered.length) { currentPage++; renderList(); }
});

// Search
document.getElementById("searchBtn").addEventListener("click", () => {
  const q = document.getElementById("searchInput").value.toLowerCase();
  filtered = emails.filter(e =>
    (e.subject || "").toLowerCase().includes(q) ||
    (e.from || "").toLowerCase().includes(q) ||
    (e.body || "").toLowerCase().includes(q)
  );
  currentPage = 1;
  renderList();
});
const composeModal = document.getElementById("composeModal");
const composeBtn = document.getElementById("composeBtn");
const closeCompose = document.getElementById("closeCompose");
const sendMail = document.getElementById("sendMail");

composeBtn.addEventListener("click", () => {
  composeModal.classList.remove("hidden");
});

closeCompose.addEventListener("click", () => {
  composeModal.classList.add("hidden");
});

sendMail.addEventListener("click", () => {
  const to = document.getElementById("composeTo").value;
  const subject = document.getElementById("composeSubject").value;
  const body = document.getElementById("composeBody").value;

  if (!to || !subject || !body) {
    alert("Please fill in all fields before sending!");
    return;
  }
  
  console.log("Sending email to:", to, "Subject:", subject, "Body:", body);
  alert("✅ Your email was 'sent' (simulated)");

  composeModal.classList.add("hidden");

  document.getElementById("composeTo").value = "";
  document.getElementById("composeSubject").value = "";
  document.getElementById("composeBody").value = "";
});


// Folder Filtering
document.querySelectorAll(".folder-btn").forEach(btn => {
  btn.addEventListener("click", () => {
    document.querySelectorAll(".folder-btn").forEach(b => b.classList.remove("active"));
    btn.classList.add("active");

    const folder = btn.dataset.folder;
    filtered = folder === "ALL" ? emails : emails.filter(e => (e.label || "").toUpperCase() === folder);
    currentPage = 1;
    renderList();
  });
});

// Dark Mode
document.getElementById("toggleDark").addEventListener("click", () => {
  document.body.classList.toggle("dark");
});

loadEmails();
